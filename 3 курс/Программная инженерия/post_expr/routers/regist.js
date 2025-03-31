const router = require('express').Router();
const {getRegistPage, postRegistPage} = require('../modules/module.js');


router.get('/', (req, res) => {
    getRegistPage(req, res)
} );

router.post('/', (req, res) => { 
    postRegistPage(req, res)
});


module.exports = router;
