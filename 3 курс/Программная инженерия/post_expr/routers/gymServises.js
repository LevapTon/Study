const router = require('express').Router();
const {getGymServisesPage, postGymServisesPage} = require('../modules/module.js');


router.get('/:id', (req, res) => {
    getGymServisesPage(req, res)
} );

router.post('/:id', (req, res) => { 
    postGymServisesPage(req, res)
});


module.exports = router;
