var mysql = require('mysql2');
require('dotenv').config();

var connection = mysql.createConnection({
    host: process.env.DB_HOST,
    port: process.env.MYSQL_BD_PORT,
    user: process.env.MYSQL_BD_USER_ROOT,
    password: process.env.MYSQL_BD_PASSWORD_ROOT,
    database: process.env.MYSQL_BD_NAME
});

connection.connect(function(err) {
    if (err) {
        console.error('Erro ao conectar ao banco de dados:', err);
        throw err;
    }
    console.log('Conectado ao banco de dados');
});
module.exports = connection;