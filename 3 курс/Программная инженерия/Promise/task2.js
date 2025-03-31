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


const processData = async (url1, url2) => {
    let popular = await get_lines_from_url(url1)
    let pricedown = await get_lines_from_url(url2)

    let ids = popular
        .map(item => item.id)

    let res = pricedown
        .filter(item => ids.includes(item.id))

    console.table(res)

    fs.writeFileSync("laptops.json", JSON.stringify(res, null, 4), 'utf8')
}


url1 = "http://files-pcoding.1gb.ru/json?filename=wb_laptops_popular.json"
url2 = "http://files-pcoding.1gb.ru/json?filename=wb_laptops_pricedown.json"

processData(url1, url2)
