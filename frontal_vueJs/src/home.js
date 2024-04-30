export default {
    props: [], 
      data: function () {
        return {
        };
      },
      mounted() {
      },
      template: '\
        <div>\
          <h1>Pages de gestion de la chaudiere</h1>\
          <p>Ce site permet de gerer la chaudiere, voici une description des differentes pages accessible depuis le bandeau supérieur:</p>\
          <ul>\
            <li>home : page de présentation ; affichée lors de l\'ouverture de ce site</li>\
            <li>calendrier : permet de fixer les périodes de mise en route de la chaudière, \
              vous disposez de 5 plages par jour sur 1 semaine, \
              pour chacune de ces plages, vous pouvez fixer les parametres suivants :</li>\
              <ul>\
                <li>heure de debut</li>\
                <li>heure de fin</li>\
                <li>consigne de chauffage</li>\
                <li>chauffage on/off</li>\
              </ul>\
            <li>temperature : permet de visualiser les differentes temperatures mesurées</li>\
              <ul>\
                <li>temperature intérieure</li>\
                <li>temperature extérieure</li>\
                <li>consigne de chauffage</li>\
              </ul>\
            <li>configuration : permet de visualiser/modifier les parametres de configuration</li>\
          </ul>\
        </div>\
      ',
      methods: {
      }
}