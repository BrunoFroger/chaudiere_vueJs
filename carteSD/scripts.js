

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
    console.log("execution appelServer");
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200){
            document.getElementById(id).innerHTML = xhttp.responseText;
        }
    };
    xhttp.open("GET", url, true);
    xhttp.send();
};

function getData(url) {
    console.log("execution getData");
    fetch(url).then((response) => {
        sleep(10).then(() =>{
            var data = response.text();
            console.log("get data => " + data);
            return data;
        });
    });
};