let totalAmount = document.getElementById("total-amount");
let userAmount = document.getElementById("user-amount");
const checkAmountButton = document.getElementById("check-amount");
const totalAmountButton = document.getElementById("total-amount-button");
const productTitle = document.getElementById("product-title");
const errorMessage = document.getElementById("budget-error");
const productTitleError = document.getElementById("product-title-error");
const amount = document.getElementById("amount");
const expenditureValue = document.getElementById("expenditure-value");
const balanceValue = document.getElementById("balance-amount");
const list = document.getElementById("list");
const expenseCategory = document.getElementById("expense-category");
let tempAmount = 0;

// Clear local storage upon refresh
window.addEventListener('load', () => {
  localStorage.clear();
});

// Set Budget Part
totalAmountButton.addEventListener("click", () => {
  tempAmount = totalAmount.value;
  if (tempAmount === "" || tempAmount < 0) {
    errorMessage.classList.remove("hide");
  } else {
    errorMessage.classList.add("hide");
    amount.innerHTML = tempAmount;
    balanceValue.innerText = tempAmount - expenditureValue.innerText;
    totalAmount.value = "";
  }
});

// Function To Disable Edit and Delete Button
const disableButtons = (bool) => {
  let editButtons = document.getElementsByClassName("edit");
  Array.from(editButtons).forEach((element) => {
    element.disabled = bool;
  });
};

// Function To Modify List Elements
const modifyElement = (element, edit = false) => {
  let parentDiv = element.parentElement;
  let currentBalance = balanceValue.innerText;
  let currentExpense = expenditureValue.innerText;
  let parentAmount = parentDiv.querySelector(".amount").innerText;
  if (edit) {
    let parentText = parentDiv.querySelector(".product").innerText;
    productTitle.value = parentText;
    userAmount.value = parentAmount;
    disableButtons(true);
  }
  balanceValue.innerText = parseInt(currentBalance) + parseInt(parentAmount);
  expenditureValue.innerText = parseInt(currentExpense) - parseInt(parentAmount);
  parentDiv.remove();
  updateChart();
};

// Function To Create List
const listCreator = (expenseName, expenseValue, category) => {
  let sublistContent = document.createElement("div");
  sublistContent.classList.add("sublist-content", "flex-space");
  list.appendChild(sublistContent);
  sublistContent.innerHTML = `<p class="product">${expenseName}</p><p class="amount">${expenseValue}</p><p class="category">${category}</p>`;
  let editButton = document.createElement("button");
  editButton.classList.add("fa-solid", "fa-pen-to-square", "edit");
  editButton.style.fontSize = "1.2em";
  editButton.addEventListener("click", () => {
    modifyElement(editButton, true);
  });
  let deleteButton = document.createElement("button");
  deleteButton.classList.add("fa-solid", "fa-trash-can", "delete");
  deleteButton.style.fontSize = "1.2em";
  deleteButton.addEventListener("click", () => {
    modifyElement(deleteButton);
  });
  sublistContent.appendChild(editButton);
  sublistContent.appendChild(deleteButton);
  document.getElementById("list").appendChild(sublistContent);
  attachEventListenersToButtons();
};

// Function To Add Expenses
checkAmountButton.addEventListener("click", () => {
  if (!userAmount.value || !productTitle.value) {
    productTitleError.classList.remove("hide");
    return false;
  }
  disableButtons(false);
  let expenditure = parseInt(userAmount.value);
  let sum = parseInt(expenditureValue.innerText) + expenditure;
  expenditureValue.innerText = sum;
  const totalBalance = tempAmount - sum;
  balanceValue.innerText = totalBalance;
  listCreator(productTitle.value, userAmount.value, expenseCategory.value);
  productTitle.value = "";
  userAmount.value = "";
  updateChart();
});

// Function to attach event listeners to edit and delete buttons
const attachEventListenersToButtons = () => {
  const editButtons = document.querySelectorAll('.edit');
  const deleteButtons = document.querySelectorAll('.delete');
  editButtons.forEach((button) => {
    button.removeEventListener('click', handleEdit);
    button.addEventListener('click', handleEdit);
  });
  deleteButtons.forEach((button) => {
    button.removeEventListener('click', handleDelete);
    button.addEventListener('click', handleDelete);
  });
};

const handleEdit = (event) => {
  modifyElement(event.target, true);
};

const handleDelete = (event) => {
  modifyElement(event.target);
};

// Function to update chart
const updateChart = () => {
  const categories = Array.from(document.querySelectorAll('.category')).map(c => c.textContent);
  const amounts = Array.from(document.querySelectorAll('.amount')).map(a => parseFloat(a.textContent));

  const ctx = document.getElementById('expenseChart').getContext('2d');
  new Chart(ctx, {
    type: 'doughnut',
    data: {
      labels: categories,
      datasets: [{
        label: 'Expenses',
        data: amounts,
        backgroundColor: ['red', 'blue', 'green', 'yellow', 'orange'],
        borderWidth: 1
      }]
    }
  });
};
