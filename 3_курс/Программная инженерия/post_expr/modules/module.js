const sqlite3 = require('sqlite3').verbose()
const sha1 = require('sha1')
const {} = require('sqlite3')


const getGymListPage = (req, res) => {
    console.log('GET "/"', req.query, req.body, req.params)
    const filt_param = req.query;
    const username = req.cookies.username || "Вход не выполнен"
    const role = req.cookies.role || 0
    const branch = req.cookies.branch || 0
    const city_filt = filt_param.city || "Без фильтра";
    const time_start_filt = filt_param.time_start || "";
    const time_end_filt = filt_param.time_end || "";
    console.log("filt_param", filt_param);
    const db = new sqlite3.Database('D:/Study/3 курс/Программная инженерия/post_expr/database/db_gym.db');
    let query = `SELECT * FROM table_branches`;
    let conditions = [];
    if (city_filt !== "Без фильтра") {
        conditions.push(`branch_city = "${city_filt}"`);
    }
    if (time_start_filt) {
        conditions.push(`branch_work_time_start >= "${time_start_filt}"`);
    }
    if (time_end_filt) {
        conditions.push(`branch_work_time_end <= "${time_end_filt}"`);
    }
    if (conditions.length > 0) {
        query += ` WHERE ` + conditions.join(' AND ');
    }
    query += ` ORDER BY branch_name`;
    db.all(query, (err, rows) => {
        if (err) {
            console.log(err.message);
            res.status(500).send("Database error");
            return;
        }
        result = [rows, username, role, branch]
        res.render('gymList.ejs', result);
        db.close();
    });
}

const postGymListPage = (req, res) => {
    console.log('POST "/:"', req.query, req.body, req.params)
    gym_id = parseInt(req.params.id.split(":")[1]) 
    const db = new sqlite3.Database('D:/Study/3 курс/Программная инженерия/post_expr/database/db_gym.db')
    let query = `DELETE FROM table_services \
        WHERE branch = ${gym_id}; \
        DELETE FROM table_branches \
        WHERE branch_id = ${gym_id};`
    db.all(query, (err, rows) => {
        if (err) console.log(err.message);
    })
    db.close()
    res.redirect('/');
}

const getGymServisesPage = (req, res) => {
    console.log('GET "/gymServises"', req.query, req.body, req.params)
    gym_id = parseInt(req.params.id.split(":")[1]) 
    const filt_param = req.query;
    const db = new sqlite3.Database('D:/Study/3 курс/Программная инженерия/post_expr/database/db_gym.db')
    const section_filt = filt_param.section || "Без фильтра";
    const price_bottom_filt = filt_param.price_bottom || "";
    const price_upper_filt = filt_param.price_upper || "";
    let query = `SELECT service_id, section_name, section_equip, section_price \
        FROM table_services \
        INNER JOIN table_sections \
        ON section = section_id \
        WHERE branch = ${gym_id} `;
    let conditions = [];
    if (section_filt !== "Без фильтра") {
        conditions.push(`section_name = "${section_filt}"`);
    }
    if (price_bottom_filt) {
        conditions.push(`section_price >= "${price_bottom_filt}"`);
    }
    if (price_upper_filt) {
        conditions.push(`section_price <= "${price_upper_filt}"`);
    }
    if (conditions.length > 0) {
        query += ` AND ` + conditions.join(' AND ');
    }
    query += ` ORDER BY section_price`;
    db.all(query, (err, rows) => {
        if (err) console.log(err.message);
        arr = rows
        result = [arr, gym_id]
        res.render('gymServises.ejs', result);
    })
    db.close()
}

const postGymServisesPage = (req, res) => {
    console.log('POST "/gymServises"', req.query, req.body, req.params)
    service_id = parseInt(req.params.id.split(":")[1]) 
    const db = new sqlite3.Database('D:/Study/3 курс/Программная инженерия/post_expr/database/db_gym.db')
    let query = `DELETE FROM table_services \
        WHERE service_id = ${service_id};`
    db.all(query, (err, rows) => {
        if (err) console.log(err.message);
    })
    db.close()
    res.redirect('/');
}

const getAddNewGymPage = (req, res) => {
    console.log('GET "/addNewGym"', req.query, req.body, req.params)
    res.render("addNewGym.ejs")
}

const postAddNewGymPage = (req, res) => {
    console.log('POST "/addNewGym"', req.query, req.body, req.params)
    let combinedDays = ''
    const db = new sqlite3.Database('D:/Study/3 курс/Программная инженерия/post_expr/database/db_gym.db')
    const workdays = req.body.workdays;
    if (!workdays) {
        combinedDays = "пн, вт, ср, чт, пт"
    }
    else {
        const selectedDays = Array.isArray(workdays) ? workdays : [workdays];
        combinedDays = selectedDays.join(', ');
    }
    query = `INSERT INTO table_branches (branch_name, branch_city, branch_address, branch_work_days, branch_work_time_start, branch_work_time_end) \
        VALUES ("${req.body.name}", "${req.body.city}", "${req.body.address}", "${combinedDays}", "${req.body.time_start}", "${req.body.time_end}")`
    db.all(query, (err) => {
        if (err) console.log(err.message);
        console.log("Создан новый спорткомплекс")
    })
    db.close()
    res.redirect("/")
}

const getAddNewGymService = (req, res) => {
    console.log('GET "/addNewGymService"', req.query, req.body, req.params)
    branch_id = parseInt(req.params.id.split(":")[1]) 
    const db = new sqlite3.Database('D:/Study/3 курс/Программная инженерия/post_expr/database/db_gym.db')
    let query = `SELECT section_id, section_name \
        FROM table_sections;`
    db.all(query, (err, rows) => {
        if (err) console.log(err.message);
        arr = rows
        result = [arr, branch_id]
        query = `SELECT section \
            FROM table_services \
            WHERE branch = ${branch_id};`
        db.all(query, (err, rows) => {
            if (err) console.log(err.message);
            arr = rows
            result.push(arr.map(x => x['section']))
            res.render('addNewGymService.ejs', result);
        })
    })
    db.close()
}

const postAddNewGymService = (req, res) => {
    console.log('POST "/addNewGymService"', req.query, req.body, req.params)
    branch_id = parseInt(req.params.id.split(":")[1]) 
    let query_val = ''
    const sections = req.body.sections
    if (!sections) {
        query_val = `(${branch_id}, 1)`
    }
    else {
        const selectedSect = Array.isArray(sections) ? sections : [sections];
        query_val = selectedSect
            .map(x => {
                return `(${branch_id}, ${x})`
            })
            .join(", ")

        query_val = [query_val, ";"].join("")
    }
    const db = new sqlite3.Database('D:/Study/3 курс/Программная инженерия/post_expr/database/db_gym.db')
    let query = `BEGIN TRANSACTION; \
        INSERT OR REPLACE INTO table_services (branch, section) \
        VALUES ${query_val} \
        CREATE TEMPORARY TABLE new_branch ( \
            branch_tmp INT, \
            section_tmp INT \
        ); \
        INSERT INTO new_branch (branch_tmp, section_tmp) \
        VALUES ${query_val} \
        DELETE FROM table_services \
        WHERE branch = ${branch_id} AND (branch, section) NOT IN ( \
            SELECT branch_tmp, section_tmp FROM new_branch); \
        DROP TABLE new_branch; \
        COMMIT;`
    db.serialize(() => {
        db.exec(query, (err) => {
            if (err) {
                console.error("Ошибка выполнения запроса:", err.message);
                return res.status(500).send("Ошибка выполнения запроса");
            }
            console.log("Секции изменены");
            db.close((err) => {
                if (err) {
                    console.error("Ошибка закрытия базы данных:", err.message);
                    return res.status(500).send("Ошибка закрытия базы данных");
                }
                res.redirect("/");
            });
        });
    });
}

const getRegistPage = (req, res) => {
    console.log('GET "/regist"', req.query, req.body, req.params)
    const db = new sqlite3.Database('D:/Study/3 курс/Программная инженерия/post_expr/database/db_gym.db')
    let query_1 = `SELECT * FROM table_roles`
    let query_2 = `SELECT branch_id, branch_name FROM table_branches`
    result = []
    db.all(query_1, (err, rows) => {
        if (err) console.log(err.message);
        arr = rows
        result.push(arr)
        db.all(query_2, (err, rows) => {
            if (err) console.log(err.message);
            arr = rows
            result.push(arr)
            res.render("regist.ejs", result)
            db.close()
        })
    })
}

const postRegistPage = (req, res) => {
    console.log('POST "/regist"', req.query, req.body, req.params)
    const db = new sqlite3.Database('D:/Study/3 курс/Программная инженерия/post_expr/database/db_gym.db')
    let query = `SELECT \
        username \
        FROM table_users \
        WHERE username == "${req.body.name}"`
    db.all(query, (err, arr) => {
        if (err) console.log(err.message);
        if (arr.length != 0) {
            let pass = sha1(req.body.password)
            query = `INSERT INTO table_users (username, password, role, branch) \
                VALUES ("${req.body.name}", "${pass}", ${parseInt(req.body.role)}, ${parseInt(req.body.gym)})`
            db.all(query, (err) => {
                if (err) console.log(err.message);
                console.log("Создан новый аккаунт")
            })
        }
        else {
            console.log("Такое имя уже используется")
        }
    })
    db.close()
    res.redirect("/")    
}

const getLoginPage = (req, res) => {
    console.log('GET "/login"', req.query, req.body, req.params)
    res.render("login.ejs")
}

const postLoginPage = (req, res) => {
    console.log('POST "/login"', req.query, req.body, req.params)
    const db = new sqlite3.Database('D:/Study/3 курс/Программная инженерия/post_expr/database/db_gym.db')
    let pass = sha1(req.body.password)
    let query = `SELECT \
        username, password, role, branch \
        FROM table_users \
        WHERE username == "${req.body.name}"`
    db.all(query, (err, arr) => {
        if (err) {
            console.log(err.message);
            res.redirect("/");
            return;
        }
        if (arr.length != 0) {
            if (pass == arr[0].password) {
                res.cookie('username', arr[0].username, { 
                    maxAge: 60_000 * 60 * 24, // one day
                });
                res.cookie('role', arr[0].role, { 
                    maxAge: 60_000 * 60 * 24, // one day
                });
                res.cookie('branch', arr[0].branch, { 
                    maxAge: 60_000 * 60 * 24, // one day
                });
                console.log("Успешный вход")
            }
        }
        res.redirect("/")
    })
    db.close()
    
}

const getUsersListPage = (req, res) => {
    console.log('GET "/usersList"', req.query, req.body, req.params)
    const filt_param = req.query;
    const db = new sqlite3.Database('D:/Study/3 курс/Программная инженерия/post_expr/database/db_gym.db')
    const role_filt = filt_param.role || "Без фильтра";
    const gym_filt = filt_param.gym || "Без фильтра";
    result = []
    conditions = []
    let query_1 = `SELECT \
        user_id, username, role_name, branch_name \
        FROM table_users \
        INNER JOIN table_roles \
        ON role = role_id \
        INNER JOIN table_branches \
        ON branch = branch_id`
    if (role_filt !== "Без фильтра") {
        conditions.push(`role_id = ${parseInt(role_filt)}`);
    }
    if (gym_filt !== "Без фильтра") {
        conditions.push(`branch_id = ${parseInt(gym_filt)}`);
    }
    if (conditions.length > 0) {
        query_1 += ` WHERE ` + conditions.join(' AND ');
    }
    db.all(query_1, (err, rows) => {
        if (err) console.log(err.message);
        result.push(rows)
        let query_2 = `SELECT \
            role_id, role_name \
            FROM table_roles`
        db.all(query_2, (err, rows_2) => {
            if (err) console.log(err.message);
            result.push(rows_2)
            let query_3 = `SELECT \
                branch_id, branch_name \
                FROM table_branches`
            db.all(query_3, (err, rows_3) => {
                if (err) console.log(err.message);
                result.push(rows_3)
                res.render("usersList.ejs", result)
                db.close()
            })
        })
    })
}

const postUsersListPage = (req, res) => {
    console.log('POST "/usersList"', req.query, req.body, req.params)
    user_id = parseInt(req.params.id.split(":")[1]) 
    const db = new sqlite3.Database('D:/Study/3 курс/Программная инженерия/post_expr/database/db_gym.db')
    let query = `DELETE FROM table_users
        WHERE user_id = ${user_id}`
    db.all(query, (err) => {
        if (err) console.log(err.message);
        db.close()
    })
    res.redirect('/')
}

const getLogout = (req, res) => {
    console.log('GET "/logout"', req.query, req.body, req.params)
    res.clearCookie('username');
    res.clearCookie('branch');
    res.clearCookie('role');
    res.redirect("/")
}

const getChangeGymPage = (req, res) => {
    console.log('GET "/changeGym"', req.query, req.body, req.params)
    branch_id = parseInt(req.params.id.split(":")[1]) 
    const db = new sqlite3.Database('D:/Study/3 курс/Программная инженерия/post_expr/database/db_gym.db')
    let query = `SELECT * \
        FROM table_branches \
        WHERE branch_id = ${branch_id}`
    db.all(query, (err, rows) => {
        if (err) console.log(err.message);
        res.render("changeGym.ejs", {result: rows[0]})
    })
    db.close()
}

const postChangeGymPage = (req, res) => {
    console.log('POST "/changeGym"', req.query, req.body, req.params)
    branch_id = parseInt(req.params.id.split(":")[1]) 
    let combinedDays = ''
    const db = new sqlite3.Database('D:/Study/3 курс/Программная инженерия/post_expr/database/db_gym.db')
    const workdays = req.body.workdays;
    if (!workdays) {
        combinedDays = "пн, вт, ср, чт, пт"
    }
    else {
        const selectedDays = Array.isArray(workdays) ? workdays : [workdays];
        combinedDays = selectedDays.join(', ');
    }
    query = `UPDATE table_branches \
        SET branch_name = "${req.body.name}", \
        branch_city = "${req.body.city}", \
        branch_address = "${req.body.address}", \
        branch_work_days = "${combinedDays}", \
        branch_work_time_start = "${req.body.time_start}", \
        branch_work_time_end = "${req.body.time_end}" \
        WHERE branch_id = ${branch_id}`
    db.all(query, (err) => {
        if (err) console.log(err.message);
        console.log("Данные о спроткомплексе изменены")
    })
    db.close()
    res.redirect("/")
}

const getUserChangePage = (req, res) => {
    console.log('GET "/userChange"', req.query, req.body, req.params)
    user_id = parseInt(req.params.id.split(":")[1]) 
    result = []
    const db = new sqlite3.Database('D:/Study/3 курс/Программная инженерия/post_expr/database/db_gym.db')
    let query_1 = `SELECT \
        user_id, username, role, branch \
        FROM table_users \
        WHERE user_id = ${user_id}`
    db.all(query_1, (err, rows) => {
        if (err) console.log(err.message);
        result.push(rows[0])
        let query_2 = `SELECT \
            role_id, role_name \
            FROM table_roles`
        db.all(query_2, (err, rows_2) => {
            if (err) console.log(err.message);
            result.push(rows_2)
            let query_3 = `SELECT \
                branch_id, branch_name \
                FROM table_branches`
            db.all(query_3, (err, rows_3) => {
                if (err) console.log(err.message);
                result.push(rows_3)
                res.render("userChange", result)
                db.close()
            })
        })
    })
}

const postUserChangePage = (req, res) => {
    console.log('POST "/userChange"', req.query, req.body, req.params)
    user_id = parseInt(req.params.id.split(":")[1]) 
    const db = new sqlite3.Database('D:/Study/3 курс/Программная инженерия/post_expr/database/db_gym.db')
    let query = `UPDATE table_users \
        SET username = '${req.body.name}', \
        role = ${parseInt(req.body.role)}, \
        branch = ${parseInt(req.body.gym)}\
        WHERE user_id = ${user_id}`
    db.all(query, (err) => {
        if (err) console.log(err.message);
        db.close()
        res.redirect('/')
    })
}


module.exports = {
    getGymListPage,
    postGymListPage,
    getGymServisesPage,
    postGymServisesPage,
    getAddNewGymPage,
    postAddNewGymPage,
    getAddNewGymService,
    postAddNewGymService,
    getRegistPage,
    postRegistPage,
    getLoginPage,
    postLoginPage,
    getUsersListPage,
    postUsersListPage,
    getLogout,
    getChangeGymPage,
    postChangeGymPage,
    getUserChangePage,
    postUserChangePage
}
