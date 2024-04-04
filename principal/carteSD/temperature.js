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
                <td>Consigne de chauffage</td>\
                <td>\
                  <input id='consigne' name='consigne' v-model='consigne' @keyup.enter='updateConsigne()'/>\
                </td>\
              </tr>\
              <tr>\
                <td>Adresse IP capteur temp int</td>\
                <td>\
                  <input id='ipTempInt' name='ipTempInt' v-model='ipTempInt' @keyup.enter='updateIpTempInt()'/>\
                </td>\
              </tr>\
              <tr>\
                <td>Adresse IP capteur temp ext</td>\
                <td>\
                  <input id='ipTempExt' name='ipTempExt' v-model='ipTempExt' @keyup.enter='updateIpTempExt()'/>\
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
        //fetch('/updateConsigne?consigne='+this.consigne).then(r => {
          this.loadDatasTemperatures();
        })
      },
      updateIpTempInt() {
        fetch('/updateIpTempInt?ip='+this.ipTempInt).then(r => {
          this.loadDatasTemperatures();
        })
      },
      updateIpTempExt() {
        fetch('/updateIpTempExt?ip='+this.ipTempExt).then(r => {
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