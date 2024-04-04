const { createApp } = Vue;

import Home from './home.js';
import Commande from './commande.js';
import Calendrier from './calendrier.js';
import Temperature from './temperature.js';
import Configuration from './configuration.js';

const app = createApp({
    data() {
        return {
	    page: 'home',
	    board: null,
	    copyright: null,
        }
    },
    mounted() {    
      this.loadDatasPiedPage();
    },
    template: '\
        <div class="entete">\
          <table>\
            <tr>\
              <td><a href="#" @click.prevent="page=\'home\'">home</a></td>\
              <td><a href="#" @click.prevent="page=\'commande\'">commande</a></td>\
              <td><a href="#" @click.prevent="page=\'calendrier\'">calendrier</a></td>\
              <td><a href="#" @click.prevent="page=\'temperature\'">temperature</a></td>\
              <td><a href="#" @click.prevent="page=\'configuration\'">configuration</a></td>\
              <td><a href="#" @click.prevent="page=\'calendrier2\'">calendrier2</a></td>\
            </tr>\
          </table>\
        </div>\
        <div>\
          <component v-bind:is="page"></component>\
        </div>\
        <div class="piedPage">\
          <p>Ce serveur est hébergé sur un arduino : {{board}}</p>\
          <i>Créé par : {{copyright}}</i>\
        </div>\
      ',
    methods: {
      loadDatasPiedPage() {
        fetch("/getDatasPiedPage").then(r => r.json()).then(response => {
          this.board = response.board;
          this.copyright = response.copyright;
        })
        .catch(error => {
          console.error(error);
        });
      },
    }
});

app.component('home', Home);
app.component('commande', Commande);
app.component('calendrier', Calendrier);
app.component('temperature', Temperature);
app.component('configuration', Configuration);
app.mount('#app');
