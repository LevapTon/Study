const router = require('express').Router();
const {getAddNewGymPage, postAddNewGymPage} = require('../modules/module.js');


router.get('/', (req, res) => {
    getAddNewGymPage(req, res)
} );

router.post('/', (req, res) => { 
    postAddNewGymPage(req, res)
});


module.exports = router;
