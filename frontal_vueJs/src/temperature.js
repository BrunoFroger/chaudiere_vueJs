export default {
    props: [],
    data: function () {
      return {
        temperatureInterieure: null,
        temperatureExterieure: null,
        ipTempInt: null,
        ipTempExt: null,
        consigne: null,
      };
    },
    mounted() {
      this.loadDatasTemperatures();
    },
    template: "\
      <div>\
        <h1>Temperatures</h1>\
      </div>\
      <div>\
        <form>\
          <table>\
            <thead>\
              <tr>\
                <th>variable</th>\
                <th>valeur</th>\
              </tr>\
            </thead>\
            <tbody>\
              <tr>\
                <td>Temperature interieure</td>\
                <td>{{this.$parent.formatTemperature(temperatureInterieure)}}</td>\
              </tr>\
              <tr>\
                <td>Temperature exterieure</td>\
                <td>{{this.$parent.formatTemperature(temperatureExterieure)}}</td>\
              </tr>\
              <tr>\
                  <td>Consigne</td>\
                  <td>{{this.$parent.formatTemperature(consigne)}}</td>\
              </tr>\
              <tr>\
                <td>Adresse IP capteur temp int</td>\
                <td>{{ipTempInt}}</td>\
              </tr>\
              <tr>\
                <td>Adresse IP capteur temp ext</td>\
                <td>{{ipTempExt}}</td>\
              </tr>\
            </tbody>\
          </table>\
        </form>\
        <button @click='loadDatasTemperatures'>refresh</button>\
      </div>\
      ",
    methods: {
      updateIpTempInt() {
        let requete = this.$parent.httpServer + "/updateIpTempInt?ip=" + this.ipTempInt
        fetch(requete).then(r => {
          this.loadDatasTemperatures();
        })
      },
      updateConsigne() {
        let requete = this.$parent.httpServer + "/updateConsigne?consigne=" + this.consigne
        fetch(requete).then(r => {
          this.loadDatasTemperatures();
        })
      },
      updateIpTempExt() {
        let requete = this.$parent.httpServer + "/updateIpTempExt?ip=" + this.ipTempExt
        fetch(requete).then(r => {
          this.loadDatasTemperatures();
        })
      },
      loadDatasTemperatures() {
        let requete = this.$parent.httpServer + "/getDatasTemperatures"
        fetch(requete).then(r => r.json()).then(response => {
        //fetch('/getDatasTemperatures').then(r => r.json()).then(response => {
          this.consigne = response.consigne;
          this.temperatureInterieure = response.temperatureInterieure;
          this.temperatureExterieure = response.temperatureExterieure;
          this.ipTempInt = response.adresseIpTempInt;
          this.ipTempExt = response.adresseIpTempExt;
          // console.log(this.ipTempInt)
          // console.log(response.adresseIpTempInt)
          if (this.ipTempInt == "") this.ipTempInt = "non connecté"
          if (this.ipTempExt == "") this.ipTempExt = "non connecté"
        })
        .catch(error => {
          console.error(error);
          this.consigne = null;
        });
      },
    }
}