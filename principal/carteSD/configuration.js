
export default {
    props: [],
    data: function () {
      return {
        environnements: {},
        configuration: {},
        ipTempInt: null,
        ipTempExt: null
      //   consigne: "NC",
      //   envSelectionne: "NC",
      //   pinRelai: "NC",
      //   heureEteHivers: "NC",
      //   chauffageOnOff: "NC",
        // adresseIpTempInt: ,
        // adresseIpTempExt: 
      //   consigneJour: "NC",
      //   consigneNuit: "NC",
      //   consigneAbsent: "NC",
      };
    },
    mounted() {
      this.loadEnvironnements();
      this.loadDatasConfig();
    },
    template: '\
      <div>\
        <h1>Configuration</h1>\
        <p>Cette page affiche les parametres contenus dans la carte SD</p>\
        <p>Pour modifier l\'environnement a utiliser ; il faut éditer le fichier de configuration sur la carte SD</p>\
      </div>\
      <div>\
        <table>\
          <thead>\
            <tr>\
              <th>variable</th>\
              <th>valeur</th>\
            </tr>\
          </thead>\
          <tbody>\
              <tr>\
                  <td>Consigne</td>\
                  <td>{{this.$parent.formatTemperature(configuration.consigne)}}</td>\
              </tr>\
              <tr>\
                  <td>Environnement utilise</td>\
                  <td>{{configuration.envSelectionne}}</td>\
              </tr>\
              <tr>\
                  <td>Pin relai</td>\
                  <td>{{configuration.pinRelai}}</td>\
              </tr>\
              <tr>\
                  <td>Regulation</td>\
                  <td>{{configuration.regulation}}</td>\
              </tr>\
              <tr>\
                  <td>Heure été/hiver</td>\
                  <td>{{configuration.heureEteHivers ? "été" : "hiver"}}</td>\
              </tr>\
              <tr>\
                  <td>chauffage</td>\
                  <td>{{configuration.chauffageOnOff ? "ON" : "OFF"}}</td>\
              </tr>\
              <tr>\
                  <td>Adresse IP du capteur de température intérieure</td>\
                  <td>\
                    <input id="ipTempInt" name="ipTempInt" v-model="ipTempInt" @keyup.enter="this.configurationUpdateIpTempInt()"/>\
                  </td>\
              </tr>\
              <tr>\
                  <td>Adresse IP du capteur de température extérieure</td>\
                  <td>\
                    <input id="ipTempExt" name="ipTempExt" v-model="ipTempExt" @keyup.enter="this.configurationUpdateIpTempExt()"/>\
                  </td>\
              </tr>\
              <tr>\
                  <td>Référence consigne jour</td>\
                  <td>{{this.$parent.formatTemperature(configuration.consigneJour)}}</td>\
              </tr>\
              <tr>\
                  <td>Référence consigne nuit</td>\
                  <td>{{this.$parent.formatTemperature(configuration.consigneNuit)}}</td>\
              </tr>\
              <tr>\
                  <td>Référence consigne absent</td>\
                  <td>{{this.$parent.formatTemperature(configuration.consigneAbsent)}}</td>\
              </tr>\
          </tbody>\
        </table>\
      </div>\
      <p>liste des environnements definis</p>\
      <div>\
        <table>\
          <thead>\
            <tr>\
              <th>id</th>\
              <th>nom</th>\
              <th>Wifi SSID</th>\
              <th>Wifi pwd</th>\
            </tr>\
          </thead>\
          <tbody>\
            <tr v-for="environnement, idx in environnements">\
              <td>{{idx}}</td>\
              <td>{{environnements[idx].nom}}</td>\
              <td>{{environnements[idx].ssid}}</td>\
              <td>{{environnements[idx].pwd}}</td>\
            </tr>\
          </tbody>\
        </table>\
      </div>\
      <div>\
        <p><a href="/listFichierConfig">  afficher fichier de configuration </a></p>\
      </div>\
    ',
    methods: {
      loadEnvironnements() {
        let requete = this.$parent.httpServer + "/environnements.json"
        fetch(requete).then(r => r.json()).then(response => {
          this.environnements=response;
          console.log("environnelents : " + this.updateDateTimeenvironnements);
        });
      },
      configurationUpdateIpTempInt() {
        console.log("updateIpTempInt => " + this.ipTempInt)
        let requete = this.$parent.httpServer + "/updateIpTempInt?ip=" + this.ipTempInt
        fetch(requete).then(r => {
          this.loadDatasConfig();
        })
      },
      configurationUpdateIpTempExt() {
        console.log("updateIpTempExt => " + this.ipTempExt)
        let requete = this.$parent.httpServer + "/updateIpTempExt?ip=" + this.ipTempExt
        fetch(requete).then(r => {
          this.loadDatasConfig();
        })
      },
      loadDatasConfig() {
        let requete = this.$parent.httpServer + "/getDatasConfig"
        fetch(requete).then(r => r.json()).then(response => {
        //fetch('/getDatasConfig').then(r => r.json()).then(response => {
          this.configuration = response
          console.log("loadDatasConfig : " + this.configuration)
          this.ipTempInt = response.adresseIpTempInt;
          this.ipTempExt = response.adresseIpTempExt;
          // this.consigne = response.consigne;
          // this.envSelectionne = response.envSelectionne;
          // this.pinRelai = response.pinRelai;
          // this.regulation = response.regulation;
          // this.heureEteHivers = response.heureEteHivers;
          // this.chauffageOnOff = response.chauffageOnOff;
          // this.adresseIpTempInt = response.adresseIpTempInt;
          // this.adresseIpTempExt = response.adresseIpTempExt;
          // this.consigneJour = response.consigneJour;
          // this.consigneNuit = response.consigneNuit;
          // this.consigneAbsent = response.consigneAbsent;
        })
        .catch(error => {
          console.error(error);
          this.configuration.consigne = null;
        });
        console.log("consigne = " + this.configuration.consigne);
      },
    }
}