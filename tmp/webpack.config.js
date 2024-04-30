const path = require('path');

module.exports = {
  entry: path.resolve(__dirname, './carteSD/main.js'),
  module: {
    rules: [
      {
        test: /\.(js)$/,
        exclude: /node_modules/
      }
    ]
  },
  resolve: {
    extensions: ['*', '.js']
  },
  output: {
    path: path.resolve(__dirname, './carteSD'),
    filename: 'bundle.js',
  },
  devServer: {
    static: path.resolve(__dirname, './carteSD'),
  },
};
