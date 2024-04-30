const http = require('node:http')
const url=require("url")
const fs=require("fs")

const hostname = 'localhost';
const port = 3000;

const rawReqToString = async (req) => {
    const buffers = [];
    for await(const chunk of req){
        buffers.push(chunk);
    }
    return Buffer.concat(buffers).toString();
};

// calendrier
var calendar = ""

// var donnees
var donnees = JSON.parse('{\
    "tempInt": 180,\
    "tempExt": 120,\
    "chauffageMode": "OFF",\
    "chauffageStatus": "OFF",\
    "circulateurOnOff": "OFF",\
    "commandeVanneChauffage":0,\
    "afficheurOnOff": "OFF",\
    "board": "simulateur Node",\
    "copyright": "B. Froger (c) depuis 2024"\
}')

// configuration
var configuration = JSON.parse('{\
    "consigne": 200,\
    "envSelectionne": "maison",\
    "pinRelai": 8,\
    "regulation": null,\
    "heureEteHivers": null,\
    "chauffageOnOff": null,\
    "adresseIpTempInt": "",\
    "adresseIpTempExt": "",\
    "consigneJour": 195,\
    "consigneNuit": 170,\
    "consigneAbsent": 135\
}')

// environnements
var environnements = ""

//=====================================================
//
//      http.createServer
//
//=====================================================
const server = http.createServer(async(req, res) => {
    //console.log("Création du serveur ....")
    //console.log('requete = ' + req.url);
    res.setHeader('Access-Control-Allow-Origin', 'http://localhost:8080');
    res.setHeader('Access-Control-Allow-Header', 'Content-Type');
    console.log("serveur.js => url = " + req.url);
    if (req.url === '/'){
        res.statusCode = 200;
        res.setHeader('Content-Type', 'text/plain; charset=utf-8');
        res.end('Hello Chaudière Vue JS\n');
    } else if (req.url.startsWith('/getDatasPiedPage')){
        let jsonData = requete_getDatasPiedPage()
        //console.log(jsonData)
        res.setHeader('Content-Type', 'text/json');
        res.end(JSON.stringify(jsonData));
    } else if (req.url.startsWith('/getDatasCommande')){
        let jsonData = requete_getDatasCommande()
        res.setHeader('Content-Type', 'text/json');
        res.end(JSON.stringify(jsonData));
    } else if (req.url.startsWith('/getDatasTemperatures')){
        let jsonData = requete_getDatasTemperatures()
        res.setHeader('Content-Type', 'text/json');
        res.end(JSON.stringify(jsonData));
    } else if (req.url.startsWith('/getDatasConfig')){
        let jsonData = requete_getDatasConfig()
        res.setHeader('Content-Type', 'text/json');
        res.end(jsonData);
    } else if (req.url.startsWith('/updateConsigne')){
        configuration.consigne = url.parse(req.url,true).query.consigne 
        res.statusCode = 200;
        res.setHeader('Content-Type', 'text/plain; charset=utf-8');
        res.end('OK\n');
        saveConfiguration();
    } else if (req.url.startsWith('/updateIpTempInt')){
        configuration.adresseIpTempInt = url.parse(req.url,true).query.ip 
        res.statusCode = 200;
        res.setHeader('Content-Type', 'text/plain; charset=utf-8');
        res.end('OK\n');
        saveConfiguration();
    } else if (req.url.startsWith('/updateIpTempExt')){
        configuration.adresseIpTempExt = url.parse(req.url,true).query.ip 
        res.statusCode = 200;
        res.setHeader('Content-Type', 'text/plain; charset=utf-8');
        res.end('OK\n');
        saveConfiguration();
    } else if (req.url.startsWith('/switchChauffageMode')){
        donnees.chauffageMode = url.parse(req.url,true).query.chauffageMode 
        // console.log("chauffageMode = " + donnees.chauffageMode)
        res.setHeader('Content-Type', 'text/plain; charset=utf-8');
        res.end(donnees.chauffageMode);
    } else if (req.url.startsWith('/switchAfficheurOnOff')){
        if (donnees.afficheurOnOff == "OFF") donnees.afficheurOnOff = "ON"
        else donnees.afficheurOnOff = "OFF"
        res.statusCode = 200;
        // console.log("afficheurOnOff = " + donnees.afficheurOnOff)
        res.setHeader('Content-Type', 'text/plain; charset=utf-8');
        res.end(donnees.afficheurOnOff);
    } else if (req.url.startsWith('/environnements.json')){
        // let jsonData = getFichierJson(contenu, "environnements.json");
        // res.setHeader('Content-Type', 'text/json');
        res.end(JSON.stringify(environnements));
    } else if (req.url.startsWith('/loadCalendrier')){
        //console.log('calendrier : ' + JSON.stringify(calendar))
        res.end(JSON.stringify(calendar));
    } else if (req.url.startsWith('/saveCalendrier')){
        //console.log('server.js => req.body : ' + req.body)
        const object = JSON.parse(await rawReqToString(req));
        saveCalendrier(object)
        res.end(JSON.stringify(calendar));
    } else if (req.url.startsWith('/saveConfiguration')){
        console.log('server.js => saveConfiguration')
        const object = JSON.parse(await rawReqToString(req));
        saveConfiguration(object)
        res.end(JSON.stringify(configuration));
    } else {
        console.log("ERREUR => " + req.url + " ")
        res.setHeader('Content-Type', 'text/plain; charset=utf-8');
        res.statusCode=404
        res.end('page not found')
    }   
});

//=====================================================
//
//      server.listen
//
//=====================================================
server.listen(port, hostname, () => {
    console.log("Execution des initialisations .....")
    console.log('Démarrage du serveur NodeJS pour chaudière Vue JS')
    console.log(`serveur => Server running at http://${hostname}:${port}/`);
    // getFichierJson(environnements, "environnements.json");
    // donnees = getFichierJson("datas.json")
    // var waitTill = new Date(new Date().getTime() + 5000);
    // while(waitTill > new Date()){}
    //const donneeJson = JSON.parse(donnees)
    //console.log("donnees = " + donnees)
    console.log("consigne = " + donnees["consigne"])
    //const environnementsJson = JSON.parse(environnements)
    // console.log("environnements = " + environnements)
    console.log("chargement du calendrier ....")
    loadCalendrier()
    loadConfiguration()
    loadEnvironnements()
    console.log("Serveur operationnel")
    refresh()
});

//=====================================================
//
//      function loadConfiguration
//
//=====================================================
function loadConfiguration(){
    let filename = "configuration.json"
    console.log("lireFichier " + filename)
    fs.readFile(filename, function(err, data) {   
        if (err)  {
            console.log("erreur ouverture fichier " + filename)
            throw err;
        } 
        console.log(filename + " chargé"); 
        configuration = JSON.parse(data);
    });
}

//=====================================================
//
//      function loadCalendrier
//
//=====================================================
function loadCalendrier(){
    let filename = "calendrier.json"
    console.log("lireFichier " + filename)
    fs.readFile(filename, function(err, data) {   
        if (err)  {
            console.log("erreur ouverture fichier " + filename)
            throw err;
        } 
        console.log(filename + " chargé"); 
        calendar = JSON.parse(data);
    });
}

//=====================================================
//
//      function loadEnvironnements
//
//=====================================================
function loadEnvironnements(){
    let filename = "environnements.json"
    console.log("lireFichier " + filename)
    fs.readFile(filename, function(err, data) {   
        if (err)  {
            console.log("erreur ouverture fichier " + filename)
            throw err;
        } 
        console.log(filename + " chargé"); 
        environnements = JSON.parse(data);
    });
}

//=====================================================
//
//      function requete_getDatasPiedPage
//
//=====================================================
function requete_getDatasCommande(){
    return {
        "consigne":configuration["consigne"],
        "chauffageMode":donnees["chauffageMode"],
        "chauffageStatus":donnees["chauffageStatus"],
        "circulateurOnOff":donnees["circulateurOnOff"],
        "commandeVanneChauffage":donnees["commandeVanneChauffage"],
        "temperatureInterieure":Math.round(donnees.tempInt),
        "temperatureExterieure":donnees["tempExt"],
        "afficheurOnOff":donnees["afficheurOnOff"],
    }
}

//=====================================================
//
//      function requete_getDatasTemperatures
//
//=====================================================
function requete_getDatasTemperatures(){
    return {
        "consigne":configuration.consigne,
        "temperatureInterieure":Math.round(donnees.tempInt),
        "temperatureExterieure":donnees.tempExt,
        "adresseIpTempInt":configuration.adresseIpTempInt,
        "adresseIpTempExt":configuration.adresseIpTempExt,
    }
}

//=====================================================
//
//      function requete_getDatasConfig
//
//=====================================================
function requete_getDatasConfig(){
    //console.log(JSON.stringify(configuration))
    return JSON.stringify(configuration)
}

//=====================================================
//
//      function requete_getDatasPiedPage
//
//=====================================================
function requete_getDatasPiedPage(){
    return {
        "board" : donnees["board"],
        "copyright" : donnees["copyright"]
    }
}

//=====================================================
//
//      function saveCalendrier
//
//=====================================================
function saveCalendrier(valeur){
    //console.log('saveCalendrier => ' + JSON.stringify(valeur))
    calendar = valeur
    // fs.writeFile('calendrier.json', 'Texte à écrire', (err) => {
    //     if (err) throw err;
    //     console.log('Le fichier a été mis à jour');
    // });
    let filename = "calendrier.json"
    console.log("save " + filename)
    fs.writeFile(filename, JSON.stringify(calendar), function(err) {   
        if (err)  {
            console.log("erreur ouverture fichier " + filename)
            throw err;
        } 
        console.log(filename + " sauvegardé"); 
    });
}

//=====================================================
//
//      function saveConfiguration
//
//=====================================================
function saveConfiguration(){
    let filename = "configuration.json"
    console.log("save " + filename)
    fs.writeFile(filename, JSON.stringify(configuration), function(err) {   
        if (err)  {
            console.log("erreur ouverture fichier " + filename)
            throw err;
        } 
        console.log(filename + " sauvegardé"); 
    });
}

//=====================================================
//
//      function checkCalendrier
//
//=====================================================
function checkCalendrier(){
    console.log('checkCalendrier => ')
    return false
}

//=====================================================
//
//      function calculRegulation
//
//=====================================================
function calculRegulation(valeur){
    console.log('calculRegulation => ')
    if (donnees.circulateurOnOff == 'OFF'){
        if (donnees.tempInt < configuration.consigne){
            donnees.circulateurOnOff = 'ON'
        }
    } else {
        if (donnees.tempInt > configuration.consigne + 2){
            donnees.commandeVanneChauffage --
            if (donnees.commandeVanneChauffage <= 0){
                donnees.circulateurOnOff = "OFF"
            }
        } 
        if (donnees.tempInt < configuration.consigne - 2){
            donnees.commandeVanneChauffage ++
            if (donnees.commandeVanneChauffage > 50){
                donnees.commandeVanneChauffage = 50
            }
        }
        if (donnees.commandeVanneChauffage > 0){
            donnees.tempInt += donnees.commandeVanneChauffage / 20
        }
    }
    if (donnees.tempInt > donnees.tempExt) {
        donnees.tempInt -= 0.1
    } else {
        donnees.tempInt = donnees.tempExt
    }
}

//=====================================================
//
//      function refresh
//
//=====================================================
function refresh(){
    if (1){
        return
    }
    console.log('refresh => --------------------------------')
    // simulation variation temp interieure
    if (donnees.tempInt > donnees.tempExt){
        donnees.tempInt -= 0.1
    }
    // test valeur du mode 
    switch(donnees.chauffageMode){
        case 'OFF':
            donnees.circulateurOnOff = 'OFF'
            donnees.commandeVanneChauffage = 0
            break
        case 'PROG':
            if (checkCalendrier() == true){
                calculRegulation()
            }
            break;
        case 'FORCE':
            donnees.circulateurOnOff = 'ON'
            donnees.commandeVanneChauffage = 50
            donnees.tempInt += 1
            break
        case 'AUTO':
            calculRegulation()
            break;
    }
    //donnees.tempInt = Math.round(donnees.tempInt)

    console.log('consigne               = ' + configuration.consigne)
    console.log('chauffageMode          = ' + donnees.chauffageMode)
    console.log('circulateurOnOff       = ' + donnees.circulateurOnOff)
    console.log('commandeVanneChauffage = ' + donnees.commandeVanneChauffage)
    console.log('tempInt                = ' + Math.round(donnees.tempInt*10)/10)
    console.log('tempExt                = ' + donnees.tempExt)
    setTimeout(refresh,500)
}