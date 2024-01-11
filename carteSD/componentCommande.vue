

//----------------------------------------
//
//      commande
//
//----------------------------------------
app.component('commande', {
  props: [],
  data: function () {
    return {
      currentDateTime: '',
      chauffageMode: null,
      chauffageStatus: null,
      modeRegulation: null,
      circulateurOnOff: null,
      commandeVanneChauffage: null,
      consigne: null,
      temperatureInterieure: null,
      afficheurOnOff: null,
    };
  },
  mounted() {
    this.updateDateTime();
    setInterval(this.updateDateTime, 1000);
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
              <form action="/switchChauffageOnOff">\
                <select name="mode" onChange="/switchChauffageOnOff">\
                  <option value="OFF" {{chauffageMode == "OFF"}} ? "selected" : "">OFF</option>\
                  <option value="PROG" {{chauffageMode == "MODE_CHAUFFAGE_PROG"}} ? "selected" : "">OFF</option>\
                  <option value="FORCE" {{chauffageMode == "MODE_CHAUFFAGE_FORCE"}} ? "selected" : "">OFF</option>\
                </select>\
                <br><br><button>Valider</button>\
              </form>\
            </td>\
            <td> activation du chauffage : \
              <br> OFF : chauffage eteint\
              <br> PROG : le calendrier (et la temperature) definissent la mise en route du circulateur \
              <br> FORCE : chauffage allumé sans régulation\
            </td>\
          </tr>\
          <tr>\
            <td> plage <a href="/calendrier">calendrier</a> </td>\
            <td align="center">{{chauffageStatus}} ? "ON" : "OFF" </td>\
            <td> ON si dans une plage active dans le calendrier</td>\
          </tr>\
          <tr>\
            <td> mode regulation </td>\
            <td align="center"> <a href="/switchModeRegulation"> {{modeRegulation}} ? "ON" : "OFF" </a></td>\
            <td> OFF : seul le calendrier pilote l\'allumage de la chaudiere<br> ON : la temperature regule l\'allulage de la chaudiere </td>\
          </tr>\
          <tr>\
            <td> circulateur </td>\
            <td align="center"> {{circulateurOnOff}} ? "ON" : "OFF" </td>\
            <td> circulateur en route si chauffage ON et dans une plage calendrier ON </td>\
          </tr>\
          <tr>\
            <td> vanne chauffage </td>\
            <td>{{commandeVanneChauffage}}</td>\
            <td> valeur de la commande appliquee a la vanne de chauffage </td>\
          </tr>\
          <tr>\
            <td> Consigne </td>\
            <td>{{consigne}}</td>\
            <td> Consigne de temperature </td>\
          </tr>\
          <tr>\
            <td> temperature interieure </td>\
            <td>{{temperatureInterieure}}</td>\
            <td> Valeur mesuree de temperature interieure </td>\
          </tr>\
          <tr>\
            <td> afficheur </td>\
            <td align="center"> <a href="/switchAfficheurOnOff"> {{afficheurOnOff}} ? "ON" : "OFF" </a></td>\
            <td> active ou non l\'afficheur de l\'arduino </td>\
          </tr>\
        </tbody>\
      </table>\
    </div>\
  ',
  methods: {
    updateDateTime() {
      const now = new Date();
      this.currentDateTime = now.toLocaleString();
    },
  }
});