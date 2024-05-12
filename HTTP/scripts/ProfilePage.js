function inject(data, ID) {
    document.getElementById(ID).innerHTML += data;
}

async function requestData(URI) {
    try {
        const response = await fetch(URI);
        const text = await response.text();
        return text;
    } catch (error) {
        console.error("Error fetching data:", error);
        return null; // or throw error if you want to handle it differently
    }
}

(async () => {
    try {
        let username = await requestData("/username");
        let email = await requestData("/email");
        let age = await requestData("/age");
        let address = await requestData("/address");

        console.log(username);
        console.log(email);
        console.log(age);
        console.log(address);

        inject(username, "username");
        inject(email, "email");
        inject(age, "age");
        inject(address, "address");
    } catch (error) {
        console.error("Error:", error);
    }
})();
