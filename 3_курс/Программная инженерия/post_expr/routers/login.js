const router = require('express').Router();
const {getLoginPage, postLoginPage} = require('../modules/module.js');


router.get('/', (req, res) => {
    getLoginPage(req, res)
} );

router.post('/', (req, res) => { 
    postLoginPage(req, res)
});


module.exports = router;
