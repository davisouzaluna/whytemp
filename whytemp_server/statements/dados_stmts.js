
var connection = require('../connect_db');

async function read_all() {
    const stmt = "SELECT * FROM dados";
    return new Promise((resolve, reject) => {
        connection.query(stmt, (err, results) => {
            if (err) {
                console.error('Erro ao executar a consulta:', err);
                return reject(err); 
            }
            resolve(results);
        });
    });
}

module.exports = {
    read_all};