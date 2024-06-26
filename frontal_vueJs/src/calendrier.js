
var DEBUG=false

export default {
    props: [],
    data: function () {
      return {
        calendar: {},
        nbPlage: 4,
        edit: {}, 
        days: ["dimanche", "lundi", "mardi", "mercredi", "jeudi", "vendredi", "samedi"]
      };
    },
    mounted() {
      this.load();
    },
    template: '\
        <div>\
        <h1>Calendrier de chauffage</h1>\
        <button @click="load">refresh</button>\
        <table>\
          <thead>\
            <tr>\
              <th></th>\
              <th colspan="5" v-for="plage in nbPlage">plage {{plage-1}}</th>\
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
                <template v-if="edit[`${idx}-${plage-1}`]">\
                  <td><input type="number" size="4" v-model.number="edit[`${idx}-${plage-1}`].heureDebut"/>:<input type="number" size="4" v-model.number="edit[`${idx}-${plage-1}`].minuteDebut"/></td>\
                  <td><input type="number" size="4" v-model.number="edit[`${idx}-${plage-1}`].heureFin"/>:<input type="number" size="4" v-model.number="edit[`${idx}-${plage-1}`].minuteFin"/></td>\
                  <td><input type="number" size="6" v-model.number="edit[`${idx}-${plage-1}`].consigne"/></td>\
                  <td><input type="checkbox" v-model.boolean="edit[`${idx}-${plage-1}`].chauffageOnOff"/></td>\
                  <td>\
                    <a href="#" @click.prevent="cancelEdit(idx, plage-1)">cancel</a>\
                    <a href="#" @click.prevent="saveEdit(idx, plage-1)">save</a>\
                  </td>\
                </template>\
                <template v-else-if="calendar && calendar[`jour${idx}`] && calendar[`jour${idx}`][`plage${plage-1}`] && calendar[`jour${idx}`][`plage${plage-1}`] && calendar[`jour${idx}`][`plage${plage-1}`].plageActive">\
                  <td>{{formatDate(calendar[`jour${idx}`][`plage${plage-1}`].heureDebut, calendar[`jour${idx}`][`plage${plage-1}`].minuteDebut)}}</td>\
                  <td>{{formatDate(calendar[`jour${idx}`][`plage${plage-1}`].heureFin, calendar[`jour${idx}`][`plage${plage-1}`].minuteFin)}}</td>\
                  <td>{{formatConsigne(calendar[`jour${idx}`][`plage${plage-1}`].consigne)}}</td>\
                  <td>{{calendar[`jour${idx}`][`plage${plage-1}`].chauffageOnOff ? "oui" : "non"}}</td>\
                  <td>\
                    <a href="#" @click.prevent="disablePlage(idx, plage-1)">disable</a>\
                    <a href="#" @click.prevent="editPlage(idx, plage-1)">edit</a>\
                    </td>\
                </template>\
                <template v-else>\
                  <td colspan="4">inactive</td>\
                  <td><a href="#" @click.prevent="editPlage(idx, plage-1)">edit</a></td>\
                </template>\
              </template>\
            </tr>\
          </tbody>\
        </table>\
        </div>\
      ',
      methods: {
        load() {
          //console.log("requete /loadCalendrier envoyee au serveur")
          let requete = this.$parent.httpServer + "/loadCalendrier"
          fetch(requete).then(r => r.json()).then(calendar => {
          //fetch('calendrier.json').then(r => r.json()).then(calendar => {
            this.calendar=calendar;
          });
        },
        save() {
          //console.log("sauvegarde du calendrier : " + JSON.stringify(this.calendar))
          let requete = this.$parent.httpServer + "/saveCalendrier"
          fetch(requete,{
            headers: {
              //'Accept': 'application/json',
              //'Content-Type': 'text/json',
              //'Access-Control-Allow-Header': 'Content-Type'
            },
            method: "POST",
            body: JSON.stringify(this.calendar)
          }).then(r => r.json()).then(calendar => {
          //fetch('calendrier.json').then(r => r.json()).then(calendar => {
            this.calendar=calendar;
          });
        },
        formatDate(heure, minute) {
          return `${String(heure).padStart(2, '0')}:${String(minute).padStart(2, '0')}`;
        },
        editPlage(jour, plage) {
          console.log("editPlage " + jour + "/" + plage)
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
            //console.log("this.edit",this.edit);
        },
        disablePlage(jour, plage) {
          console.log("disablePlage " + jour + "/" + plage)
          let active=true;
          if (this.calendar && this.calendar[`jour${jour}`] && this.calendar[`jour${jour}`][`plage${plage}`] && this.calendar[`jour${jour}`][`plage${plage}`].plageActive) {
            active=false;
            delete(this.edit[`${jour}-${plage}`]);
          }
          if (DEBUG) {
            this.calendar[`jour${jour}`][`plage${plage}`].plageActive=active;
          } else {
            console.log("disablePlage " + jour + "/" + plage)
            //send to backend
            delete(this.calendar[`jour${jour}`][`plage${plage}`]);
            this.save()
            this.load();
          }
        },
        cancelEdit(jour, plage) {
          console.log("cancelEdit " + jour + "/" + plage)
          delete(this.edit[`${jour}-${plage}`]);
          this.save()
        },
        saveEdit(jour, plage) {
          console.log("saveEdit " + jour + "/" + plage)
          //console.log("save Calendrier", `jour${jour}`, `plage${plage}`, JSON.stringify(this.edit[`${jour}-${plage}`], null, 2));
          if (DEBUG) {
            this.calendar[`jour${jour}`][`plage${plage}`]=this.edit[`${jour}-${plage}`];
            delete(this.edit[`${jour}-${plage}`]);
            //this.save()
          } else {
            //send to backend
            this.calendar[`jour${jour}`][`plage${plage}`]=this.edit[`${jour}-${plage}`];
            delete(this.edit[`${jour}-${plage}`]);
            //this.load();
            this.save()
          }
        },
        formatConsigne(consigne) {
          return `${consigne/10}.${consigne%10} °C`;
        },
      }
}