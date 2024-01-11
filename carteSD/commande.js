export default {
    props: [],
    data: function () {
      return {
        currentDateTime: '',
        consigne: null,
        chauffageMode: "OFF",
        chauffageStatus: null,
        circulateurOnOff: null,
        commandeVanneChauffage: null,
        temperatureInterieure: null,
        temperatureExterieure: null,
        afficheurOnOff: null,
      };
    },
    mounted() {
      this.updateDateTime();
      setInterval(this.updateDateTime, 1000);
      this.loadDatasCommande();
    },
    template: '\
      <div>\
        <h1>Page décrivant les commandes de chauffage</h1>\
        <p>Nous somme le {{currentDateTime}}</p>\
      </div>\
      <div>\
        <table>\
          <thead>\
              <tr>\
                  <th> variable </th>\
                  <th> valeur </th>\
                  <th> commentaire </th>\
              </tr>\
          </thead>\
          <tbody>\
            <tr>\
              <td> chauffage </td>\
              <td align="center">\
                <form>\
                  <select name="modeChauffage" v-model="chauffageMode" @change="switchChauffageMode()">\
                    <option value="OFF">OFF</option>\
                    <option value="PROG">PROG</option>\
                    <option value="FORCE">FORCE</option>\
                    <option value="MANUEL">MANUEL</option>\
                  </select>\
                </form>\
              </td>\
              <td> activation du chauffage : \
                <br> OFF : chauffage eteint\
                <br> PROG : le calendrier (et la temperature) definissent la mise en route du circulateur\
                <br> FORCE : le chauffage est allumé sans régulation\
                <br> MANUEL : seule la temperature regule le chauffage\
              </td>\
            </tr>\
            <tr>\
              <td> plage calendrier </td>\
              <td align="center">{{chauffageStatus}}</td>\
              <td> ON si dans une plage active dans le calendrier</td>\
            </tr>\
            <tr>\
              <td> circulateur </td>\
              <td align="center"> {{circulateurOnOff}} </td>\
              <td> circulateur en route si chauffage ON et dans une plage calendrier ON </td>\
            </tr>\
            <tr>\
              <td> vanne chauffage </td>\
              <td align="center">{{commandeVanneChauffage}}</td>\
              <td> valeur de la commande appliquee a la vanne de chauffage </td>\
            </tr>\
            <tr>\
              <td> Consigne </td>\
              <td align="center">{{consigne}}</td>\
              <td> Consigne de temperature </td>\
            </tr>\
            <tr>\
              <td> temperature interieure </td>\
              <td align="center">{{temperatureInterieure}}</td>\
              <td> Valeur mesuree de temperature interieure </td>\
            </tr>\
            <tr>\
              <td> temperature exterieure </td>\
              <td align="center">{{temperatureExterieure}}</td>\
              <td> Valeur mesuree de temperature iexterieure </td>\
            </tr>\
            <tr>\
              <td> afficheur </td>\
              <td align="center"> <a href="/switchAfficheurOnOff"> {{afficheurOnOff}} </a></td>\
              <td> active ou non l\'afficheur de l\'arduino </td>\
            </tr>\
          </tbody>\
        </table>\
        <button @click="loadDatasCommande">refresh</button>\
      </div>\
    ',
    methods: {
      updateDateTime() {
        const now = new Date();
        this.currentDateTime = now.toLocaleString();
      },
      switchModeRegulation() {
        fetch('/switchModeRegulation').then(r => {
          this.loadDatasCommande();
        })
      },
      switchChauffageMode() {
        fetch('/switchChauffageMode?mode='+this.chauffageMode).then(r => {
          this.loadDatasCommande();
        })
      },
      loadDatasCommande() {
        fetch('/getDatasCommande').then(r => r.json()).then(response => {
          this.consigne = response.consigne;
          this.chauffageMode = response.chauffageMode;
          this.chauffageStatus = response.chauffageStatus;
          this.circulateurOnOff = response.circulateurOnOff;
          this.commandeVanneChauffage = response.commandeVanneChauffage;
          this.temperatureInterieure = response.temperatureInterieure;
          this.temperatureExterieure = response.temperatureExterieure;
          this.afficheurOnOff = response.afficheurOnOff;
        })
        .catch(error => {
          console.error(error);
          this.consigne = null;
        });
      },
    }
}