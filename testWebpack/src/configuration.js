export default {
    props: [],
    data: function () {
      return {
        currentDateTime: '',
        environnements: {},
        consigne: "NC",
        envSelectionne: "NC",
        pinRelai: "NC",
        heureEteHivers: "NC",
        chauffageOnOff: "NC",
        adresseIpTempInt: "NC",
        consigneJour: "NC",
        consigneNuit: "NC",
        consigneAbsent: "NC",
      };
    },
    mounted() {
      this.loadEnvironnements();
      this.loadDatasConfig();
      this.updateDateTime();
      setInterval(this.updateDateTime, 1000);
    },
    template: '\
      <div>\
        <h1>Configuration</h1>\
        <p>Nous somme le {{currentDateTime}}</p>\
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
                  <td>{{formatConsigne(consigne)}}</td>\
              </tr>\
              <tr>\
                  <td>Environnement utilise</td>\
                  <td>{{envSelectionne}}</td>\
              </tr>\
              <tr>\
                  <td>Pin relai</td>\
                  <td>{{pinRelai}}</td>\
              </tr>\
              <tr>\
                  <td>Heure été/hiver</td>\
                  <td>{{heureEteHivers ? "été" : "hivers"}}</td>\
              </tr>\
              <tr>\
                  <td>chauffage</td>\
                  <td>{{chauffageOnOff ? "ON" : "OFF"}}</td>\
              </tr>\
              <tr>\
                  <td>Adresse capteur temp interieure</td>\
                  <td>\
                    <form >\
                      <input id="ipTempInt" name="ipTempInt" valeur="{{adresseIpTempInt}}">\
                    </form>\
                  </td>\
              </tr>\
              <tr>\
                  <td>Référence consigne jour</td>\
                  <td>{{formatConsigne(parseInt(consigneJour))}}</td>\
              </tr>\
              <tr>\
                  <td>Référence consigne nuit</td>\
                  <td>{{formatConsigne(parseInt(consigneNuit))}}</td>\
              </tr>\
              <tr>\
                  <td>Référence consigne absent</td>\
                  <td>{{formatConsigne(parseInt(consigneAbsent))}}</td>\
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
        <p><a href="/sauveConfig">  sauver la configuration </a></p>\
      </div>\
      <div>\
        <p><a href="/listFichierConfig">  afficher fichier de configuration </a></p>\
      </div>\
    ',
    methods: {
      loadEnvironnements() {
      fetch('environnements.json').then(r => r.json()).then(environnements => {
        this.environnements=environnements;
        console.log("environnelents : " + environnements);
        });
      },
      loadDatasConfig() {
        fetch('/getDatasConfig').then(r => r.json()).then(response => {
          this.consigne = response.consigne;
          this.envSelectionne = response.envSelectionne;
          this.pinRelai = response.pinRelai;
          this.regulation = response.regulation;
          this.heureEteHivers = response.heureEteHivers;
          this.chauffageOnOff = response.chauffageOnOff;
          this.adresseIpTempInt = response.adresseIpTempInt;
          this.consigneJour = response.consigneJour;
          this.consigneNuit = response.consigneNuit;
          this.consigneAbsent = response.consigneAbsent;
        })
        .catch(error => {
          console.error(error);
          this.consigne = null;
        });
        console.log("consigne = " + this.consigne);
      },
      formatConsigne(consigne) {
        return `${consigne/10}.${consigne%10} °C`;
      },
      updateDateTime() {
        const now = new Date();
        this.currentDateTime = now.toLocaleString();
      },
    }
}