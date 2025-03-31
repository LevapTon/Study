const request = require('request') // npm i request
const fs = require('fs')


const get_lines_from_url = (url) => {
    return new Promise((resolve, reject) => {
        request.get(url, (error, response, data) => {
            if (error) reject(error)
            resolve(JSON.parse(data))
        })
    })
}


const processData = async (url) => {
    let lines = await get_lines_from_url(url)
    lines = lines
        .map(obj => {
            return {
                id: obj.id,
                name: obj.name,
                phone: obj.phone,
                city: obj.address.city
            }
        })
        .sort((a, b) => a.name > b.name? +1: -1)

    console.table(lines)
    fs.writeFileSync("users.json", JSON.stringify(lines, null, 4), 'utf8')
}


url = "http://files-pcoding.1gb.ru/json?filename=users.json"
processData(url)
