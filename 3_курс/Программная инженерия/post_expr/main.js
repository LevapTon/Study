const express = require('express'), // npm i express
    app = express(),
    cookieParser = require('cookie-parser')
    HOST = 'localhost'
    PORT = 3000,
    rt_gym_list = require('./routers/gymList.js');
    rt_gym_servises = require('./routers/gymServises.js');
    rt_new_gym = require('./routers/addNewGym.js');
    rt_new_gym_service = require('./routers/addNewGymService.js');
    rt_regist = require('./routers/regist.js');
    rt_login = require('./routers/login.js');
    rt_users_list = require('./routers/usersList.js');
    rt_logout = require('./routers/logout.js');
    rt_change_gym = require('./routers/changeGym.js');
    rt_user_change = require('./routers/userChange.js');
    

app.use('/css', express.static('css'));
app.use(cookieParser());
app.use(express.json());
app.use(express.urlencoded({ extended: true })); // middleware for parsing Object from URL and body from Html
app.set('view engine', 'ejs');
app.use(express.json()); // middleware для распознавания объектов
app.use('/addNewGymService', rt_new_gym_service)
app.use('/gymServises', rt_gym_servises)
app.use('/userChange', rt_user_change)
app.use('/usersList', rt_users_list)
app.use('/addNewGym', rt_new_gym)
app.use('/changeGym', rt_change_gym)
app.use('/regist', rt_regist)
app.use('/logout', rt_logout)
app.use('/login', rt_login)
app.use('/', rt_gym_list)
app.listen(PORT, HOST, () => console.log(`http://${HOST}:${PORT}/`));
