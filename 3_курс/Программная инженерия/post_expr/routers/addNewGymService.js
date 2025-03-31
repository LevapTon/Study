const router = require('express').Router();
const {getAddNewGymService, postAddNewGymService} = require('../modules/module.js');


router.get('/:id', (req, res) => {
    getAddNewGymService(req, res)
} );

router.post('/:id', (req, res) => { 
    postAddNewGymService(req, res)
});


module.exports = router;
