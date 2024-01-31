function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
};

function appelServer(url, cFuntion) {
    console.log("execution appelServer");
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200){
            cFuntion(this);
        }
    };
    xhttp.open("GET", url, true);
    xhttp.send();
};

function updateData(url, id) {
    console.log("execution updateData");
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200){
            document.getElementById(id).innerHTML = xhttp.responseText;
            document.getElementById(id).innerHTML = xhttp.responseText;
        }
    };
    xhttp.open("GET", url, true);
    xhttp.send();
};

function getData(url) {
    console.log("execution getData");
    let data = fetch(url).then((data) => {
        return data;
    });
};

function getJson(url) {
    console.log("execution getJson");
    fetch(url)
        .then((response) => {
            if(!response.ok) {
                throw new Error ('HTTP error; status = $(response.status}');
            }
            return data;
        })
        .then ((data) => {
            return data;
        });
};