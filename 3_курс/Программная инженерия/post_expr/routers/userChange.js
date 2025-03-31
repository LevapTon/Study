const router = require('express').Router();
const {getUserChangePage, postUserChangePage} = require('../modules/module.js');


router.get('/:id', (req, res) => {
    getUserChangePage(req, res)
} );

router.post('/:id', (req, res) => { 
    postUserChangePage(req, res)
});


module.exports = router;
