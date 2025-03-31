const router = require('express').Router();
const {getLogout} = require('../modules/module.js');


router.get('/', (req, res) => {
    getLogout(req, res)
} );

// router.post('/', (req, res) => { 
//     postLoginPage(req, res)
// });


module.exports = router;
