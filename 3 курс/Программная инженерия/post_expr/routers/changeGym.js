const router = require('express').Router();
const {getChangeGymPage, postChangeGymPage} = require('../modules/module.js');


router.get('/:id', (req, res) => {
    getChangeGymPage(req, res)
} );

router.post('/:id', (req, res) => { 
    postChangeGymPage(req, res)
});


module.exports = router;
