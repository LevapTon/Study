const router = require('express').Router();
const {getUsersListPage, postUsersListPage} = require('../modules/module.js');


router.get('/', (req, res) => {
    getUsersListPage(req, res)
} );

router.post('/', (req, res) => { 
    postUsersListPage(req, res)
});


module.exports = router;
