export default {
    props: [],
    data: function () {
      return {
        currentDateTime: '',
        calendar: {},
        nbPlage: 4,
        edit: {}, 
        days: ["dimanche", "lundi", "mardi", "mercredi", "jeudi", "vendredi", "samedi"]
      };
    },
    mounted() {
      console.log("mounted");
      this.load();
      this.updateDateTime();
      setInterval(this.updateDateTime, 1000);
    },
    template: '\
        <div>\
        <h1>Calendrier de chauffage</h1>\
        <p>Nous somme le {{currentDateTime}}</p>\
        <table>\
          <thead>\
            <tr>\
              <th></th>\
              <th colspan="5" v-for="plage in nbPlage">plage {{plage}}</th>\
            </tr>\
            <tr>\
              <th></th>\
              <template v-for="plage in nbPlage">\
                <th>début</th>\
                <th>fin</th>\
                <th>consigne</th>\
                <th>actif?</th>\
                <th>action</th>\
              </template>\
            </tr>\
          </thead>\
          <tbody>\
            <tr v-for="day, idx in days">\
              <td>{{day}}</td>\
              <template v-for="plage in nbPlage">\
                <template v-if="edit[`${idx}-${plage}`]">\
                  <td><input type="number" size="4" v-model.number="edit[`${idx}-${plage}`].heureDebut"/>:<input type="number" size="4" v-model.number="edit[`${idx}-${plage}`].minuteDebut"/></td>\
                  <td><input type="number" size="4" v-model.number="edit[`${idx}-${plage}`].heureFin"/>:<input type="number" size="4" v-model.number="edit[`${idx}-${plage}`].minuteFin"/></td>\
                  <td><input type="number" size="6" v-model.number="edit[`${idx}-${plage}`].consigne"/></td>\
                  <td><input type="checkbox" v-model.boolean="edit[`${idx}-${plage}`].chauffageOnOff"/></td>\
                  <td>\
                    <a href="#" @click.prevent="cancelEdit(idx, plage)">cancel</a>\
                    <a href="#" @click.prevent="saveEdit(idx, plage)">save</a>\
                  </td>\
                </template>\
                <template v-else-if="calendar && calendar[`jour${idx}`] && calendar[`jour${idx}`][`plage${plage}`] && calendar[`jour${idx}`][`plage${plage}`] && calendar[`jour${idx}`][`plage${plage}`].plageActive">\
                  <td>{{formatDate(calendar[`jour${idx}`][`plage${plage}`].heureDebut, calendar[`jour${idx}`][`plage${plage}`].minuteDebut)}}</td>\
                  <td>{{formatDate(calendar[`jour${idx}`][`plage${plage}`].heureFin, calendar[`jour${idx}`][`plage${plage}`].minuteFin)}}</td>\
                  <td>{{formatConsigne(calendar[`jour${idx}`][`plage${plage}`].consigne)}}</td>\
                  <td>{{calendar[`jour${idx}`][`plage${plage}`].chauffageOnOff ? "oui" : "non"}}</td>\
                  <td>\
                    <a href="#" @click.prevent="disablePlage(idx, plage)">disable</a>\
                    <a href="#" @click.prevent="editPlage(idx, plage)">edit</a>\
                    </td>\
                </template>\
                <template v-else>\
                  <td colspan="4">inactive</td>\
                  <td><a href="#" @click.prevent="editPlage(idx, plage)">edit</a></td>\
                </template>\
              </template>\
            </tr>\
          </tbody>\
        </table>\
        </div>\
      ',
      methods: {
        load() {
          fetch('calendrier.json').then(r => r.json()).then(calendar => {
            this.calendar=calendar;
          });
        },
        formatDate(heure, minute) {
          return `${String(heure).padStart(2, '0')}:${String(minute).padStart(2, '0')}`;
        },
        editPlage(jour, plage) {
            if (this.calendar && this.calendar[`jour${jour}`] && this.calendar[`jour${jour}`][`plage${plage}`] && this.calendar[`jour${jour}`][`plage${plage}`].plageActive) {
          this.edit[`${jour}-${plage}`]=this.calendar[`jour${jour}`][`plage${plage}`];
            } else {
          this.edit[`${jour}-${plage}`]={
                "modele":0,
                "heureDebut":0,
                "minuteDebut":0,
                "heureFin":23,
                "minuteFin":59,
                "chauffageOnOff":false,
                "consigne":200,
                "plageActive":true
              };
            }
            console.log("this.edit",this.edit);
        },
        disablePlage(jour, plage) {
          let active=true;
          if (this.calendar && this.calendar[`jour${jour}`] && this.calendar[`jour${jour}`][`plage${plage}`] && this.calendar[`jour${jour}`][`plage${plage}`].plageActive) {
            active=false;
          }
          if (DEBUG) {
            this.calendar[`jour${jour}`][`plage${plage}`].plageActive=active;
          } else {
            //send to backend
            this.load();
          }
        },
        cancelEdit(jour, plage) {
          delete(this.edit[`${jour}-${plage}`]);
        },
        saveEdit(jour, plage) {
          console.log("saveConfig", `jour${jour}`, `plage${plage}`, JSON.stringify(this.edit[`${jour}-${plage}`], null, 2));
          if (DEBUG) {
            this.calendar[`jour${jour}`][`plage${plage}`]=this.edit[`${jour}-${plage}`];
            delete(this.edit[`${jour}-${plage}`]);
          } else {
            //send to backend
            delete(this.edit[`${jour}-${plage}`]);
            this.load();
          }
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