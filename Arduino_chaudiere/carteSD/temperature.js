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
                <td>Consigne de chauffage (en 1/10° de degré : 200=20.0°C)</td>\
                <td>\
                  <input id='consigne' name='consigne' v-model='consigne' @keyup.enter='updateConsigne()'/>\
                </td>\
              </tr>\
              <tr>\
                <td>Adresse IP capteur temp int</td>\
                <td>\
                  <input id='ipTempInt' name='ipTempInt' v-model='ipTempInt' @keyup.enter='this.updateIpTempInt()'/>\
                </td>\
              </tr>\
              <tr>\
                <td>Adresse IP capteur temp ext</td>\
                <td>\
                  <input id='ipTempExt' name='ipTempExt' v-model='ipTempExt' @keyup.enter='this.updateIpTempExt()'/>\
                </td>\
              </tr>\
            </tbody>\
          </table>\
        </form>\
        <button @click='loadDatasTemperatures'>refresh</button>\
      </div>\
      ",
    methods: {
      updateConsigne() {
        let requete = this.$parent.httpServer + "/updateConsigne?consigne=" + this.consigne
        fetch(requete).then(r => r.json()).then(response => {
          this.loadDatasTemperatures();
        })
      },
      updateIpTempInt() {
        let requete = this.$parent.httpServer + "/updateIpTempInt?ip=" + this.ipTempInt
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
          this.ipTempInt = response.ipTempInt;
          this.ipTempExt = response.ipTempExt;
        })
        .catch(error => {
          console.error(error);
          this.consigne = null;
        });
      },
    }
}