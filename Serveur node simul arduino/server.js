const http = require('node:http')
const url=require("url")
const fs=require("fs")
//const bodyParser = require('body-parser')

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
var calendar = JSON.parse('{ \
    "jour0": { \
        "plage0": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage1": {\
            "modele":null,\
            "heureDebut":12,\
            "minuteDebut":0,\
            "heureFin":13,\
            "minuteFin":0,\
            "chauffageOnOff":true,\
            "consigne":19,\
            "plageActive":true\
        }, \
        "plage2": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage3": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage4": {\
            "modele":0,\
            "heureDebut":0,\
            "minuteDebut":0,\
            "heureFin":23,\
            "minuteFin":59,\
            "chauffageOnOff":false,\
            "consigne":200,\
            "plageActive":true\
        }\
    }\
    , "jour1": { \
        "plage0": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage1": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage2": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage3": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage4": {\
            "modele":0,\
            "heureDebut":0,\
            "minuteDebut":0,\
            "heureFin":23,\
            "minuteFin":59,\
            "chauffageOnOff":false,\
            "consigne":200,\
            "plageActive":true\
        }\
    }\
    , "jour2": { \
        "plage0": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage1": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage2": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage3": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage4": {\
            "modele":0,\
            "heureDebut":0,\
            "minuteDebut":0,\
            "heureFin":23,\
            "minuteFin":59,\
            "chauffageOnOff":false,\
            "consigne":200,\
            "plageActive":true\
        }\
    }\
    , "jour3": { \
        "plage0": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage1": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage2": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage3": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage4": {\
            "modele":0,\
            "heureDebut":0,\
            "minuteDebut":0,\
            "heureFin":23,\
            "minuteFin":59,\
            "chauffageOnOff":false,\
            "consigne":200,\
            "plageActive":true\
        }\
    }\
    , "jour4": { \
        "plage0": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage1": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage2": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage3": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage4": {\
            "modele":0,\
            "heureDebut":0,\
            "minuteDebut":0,\
            "heureFin":23,\
            "minuteFin":59,\
            "chauffageOnOff":false,\
            "consigne":200,\
            "plageActive":true\
        }\
    }\
    , "jour5": { \
        "plage0": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage1": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage2": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage3": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage4": {\
            "modele":0,\
            "heureDebut":0,\
            "minuteDebut":0,\
            "heureFin":23,\
            "minuteFin":59,\
            "chauffageOnOff":false,\
            "consigne":200,\
            "plageActive":true\
        }\
    }\
    , "jour6": { \
        "plage0": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage1": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage2": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage3": {\
            "modele":null,\
            "heureDebut":null,\
            "minuteDebut":null,\
            "heureFin":null,\
            "minuteFin":null,\
            "chauffageOnOff":false,\
            "consigne":null,\
            "plageActive":false\
        }, \
        "plage4": {\
            "modele":0,\
            "heureDebut":0,\
            "minuteDebut":0,\
            "heureFin":23,\
            "minuteFin":59,\
            "chauffageOnOff":false,\
            "consigne":200,\
            "plageActive":true\
        }\
    }\
}')

// var donnees
var donnees = JSON.parse('{\
    "consigne": 200,\
    "tempInt": 180,\
    "tempExt": 120,\
    "chauffageMode": "OFF",\
    "chauffageStatus": "OFF",\
    "circulateurOnOff": "OFF",\
    "commandeVanneChauffage":0,\
    "afficheurOnOff": "OFF",\
    "envSelectionne": "maison",\
    "pinRelai": 8,\
    "regulation": null,\
    "heureEteHivers": null,\
    "chauffageOnOff": null,\
    "adresseIpTempInt": null,\
    "adresseIpTempExt": null,\
    "consigneJour": 195,\
    "consigneNuit": 170,\
    "consigneAbsent": 135,\
    "board": "simulateur Node",\
    "copyright": "B. Froger (c) depuis 2024"\
}')

var environnements = JSON.parse('[\
    {\
        "nom": "maison",\
        "ssid": "Livebox-A0EA",\
        "pwd": "eHQSXY2DHrrGb3AXsz"\
    },\
    {\
        "nom": "bureau",\
        "ssid": "Livebox-webcom",\
        "pwd": "webcom2014"\
    },\
    {\
        "nom": "tp_link",\
        "ssid": "TP-LINK_88E2",\
        "pwd": "75546007"\
    },\
    {\
        "nom": "pixel6",\
        "ssid": "Pixel_6_Bruno",\
        "pwd": "0296911369"\
    }\
]')

// var consigne = 200;
// var tempInt = 180
// var tempExt = 120
// var chauffageMode = "OFF"
// var chauffageStatus = "OFF"
// var circulateurOnOff = "OFF"
// var afficheurOnOff = "OFF"
// var adresseIpTempInt = "NC"
// var adresseIpTempExt = "NC"
// var envSelectionne = null
// var pinRelai =  8
// var regulation =  null
// var heureEteHivers =  null
// var chauffageOnOff =  null
// var adresseIpTempInt =  null
// var adresseIpTempExt =  null
// var consigneJour =  195
// var consigneNuit =  170
// var consigneAbsent =  135

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
        res.end(JSON.stringify(jsonData));
    } else if (req.url.startsWith('/updateConsigne')){
        donnees.consigne = url.parse(req.url,true).query.consigne 
        res.statusCode = 200;
        res.setHeader('Content-Type', 'text/plain; charset=utf-8');
        res.end('OK\n');
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
    } else {
        res.setHeader('Content-Type', 'text/plain; charset=utf-8');
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
    console.log("donnees = " + donnees)
    console.log("consigne = " + donnees["consigne"])
    //const environnementsJson = JSON.parse(environnements)
    // console.log("environnements = " + environnements)
    console.log("Serveur operationnel")
});

//=====================================================
//
//      function requete_getDatasPiedPage
//
//=====================================================
function requete_getDatasCommande(){
    return {
        "consigne":donnees["consigne"],
        "chauffageMode":donnees["chauffageMode"],
        "chauffageStatus":donnees["chauffageStatus"],
        "circulateurOnOff":donnees["circulateurOnOff"],
        "commandeVanneChauffage":donnees["commandeVanneChauffage"],
        "temperatureInterieure":donnees["tempInt"],
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
        "consigne":donnees["consigne"],
        "temperatureInterieure":donnees["tempInt"],
        "temperatureExterieure":donnees["tempExt"],
        "ipTempInt":donnees["adresseIpTempInt"],
        "ipTempExt":donnees["adresseIpTempExt"],
    }
}

//=====================================================
//
//      function requete_getDatasConfig
//
//=====================================================
function requete_getDatasConfig(){
    return {
        "consigne":donnees["consigne"],
        "envSelectionne":donnees["envSelectionne"],
        "pinRelai":donnees["pinRelai"],
        "regulation":donnees["regulation"],
        "heureEteHivers":donnees["heureEteHivers"],
        "chauffageOnOff":donnees["chauffageOnOff"],
        "adresseIpTempInt":donnees["adresseIpTempInt"],
        "adresseIpTempExt":donnees["adresseIpTempExt"],
        "consigneJour":donnees["consigneJour"],
        "consigneNuit":donnees["consigneNuit"],
        "consigneAbsent":donnees["consigneAbsent"],
    }
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
//      function savaCalendrier
//
//=====================================================
function saveCalendrier(valeur){
    console.log('saveCalendrier => ' + JSON.stringify(valeur))
    calendar = valeur
}