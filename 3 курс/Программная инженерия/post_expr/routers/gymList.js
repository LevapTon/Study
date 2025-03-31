const router = require('express').Router();
const {getGymListPage, postGymListPage} = require('../modules/module.js');


router.get('/', (req, res) => {
    getGymListPage(req, res)
} );

router.post('/:id', (req, res) => { 
    postGymListPage(req, res)
});


module.exports = router;
