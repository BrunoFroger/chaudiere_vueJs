export default {
    props: [],
    data: function () {
      return {
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
      this.loadDatasCommande();
    },
    template: '\
      <div>\
        <h1>Commandes de chauffage</h1>\
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
                    <option value="AUTO">AUTO</option>\
                  </select>\
                </form>\
              </td>\
              <td> activation du chauffage : \
                <br> OFF : chauffage eteint\
                <br> PROG : le calendrier (et la temperature) definissent la mise en route du circulateur\
                <br> FORCE : le chauffage est allumé sans régulation\
                <br> AUTO : la temperature regule le chauffage\
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
              <td align="center">{{this.$parent.formatTemperature(consigne)}}</td>\
              <td> Consigne de temperature </td>\
            </tr>\
            <tr>\
              <td> temperature interieure </td>\
              <td align="center">{{this.$parent.formatTemperature(temperatureInterieure)}}</td>\
              <td> Valeur mesuree de temperature interieure </td>\
            </tr>\
            <tr>\
              <td> temperature exterieure </td>\
              <td align="center">{{this.$parent.formatTemperature(temperatureExterieure)}}</td>\
              <td> Valeur mesuree de temperature iexterieure </td>\
            </tr>\
            <tr>\
              <td> afficheur </td>\
              <td align="center" @click="switchAfficheurOnOff">{{afficheurOnOff}}</td>\
              <td> active ou non l\'afficheur de l\'arduino </td>\
            </tr>\
          </tbody>\
        </table>\
        <button @click="loadDatasCommande">refresh</button>\
      </div>\
    ',
    methods: {
      switchChauffageMode() {
        let requete = this.$parent.httpServer + "/switchChauffageMode?chauffageMode=" + this.chauffageMode
        fetch(requete).then(r => {
          this.loadDatasCommande();
        })
      },
      switchModeRegulation() {
        let requete = this.$parent.httpServer + "/switchModeRegulation"
        fetch(requete).then(r => {
          this.loadDatasCommande();
        })
      },
      switchAfficheurOnOff() {
        let requete = this.$parent.httpServer + "/switchAfficheurOnOff"
        fetch(requete).then(r => {
        //fetch('/switchChauffageMode?mode='+this.chauffageMode).then(r => {
          this.loadDatasCommande();
        })
      },
      loadDatasCommande() {
        let requete = this.$parent.httpServer + "/getDatasCommande"
        fetch(requete).then(r => r.json()).then(response => {
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