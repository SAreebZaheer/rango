// Function Declarations

function inject(data, ID){
	document.getElementById(ID).innerHTML += data;
}

function requestData(URI){
	fetch(URI)
  .then((response) => response.text())
  .then((text) => console.log(text));
}

console.log("We have life!")
requestData("/PROTECTED/Azeem/UserData.XML")
console.log("Womp womp")