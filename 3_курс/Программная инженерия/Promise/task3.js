const _ = require('lodash')
const request = require('request') // npm i request
const fs = require('fs')


const write_to_csv = (filename, arr, fields, sep=';') => {
    let lines = arr
        .map(x => fields.map(field => x[field]).join(sep))
    lines
        .unshift(fields.join(sep))
    fs
        .writeFileSync(filename, lines.join('\n'), { encoding: 'utf8'}) 
}


const get_lines_from_url = (url) => {
    return new Promise((resolve, reject) => {
        request.get(url, (error, response, data) => {
            if (error) reject(error)
            resolve(JSON.parse(data))
        })
    })
}


const processData = async (url, query) => {
    let lines = await get_lines_from_url(url)

    const res = lines
        .filter((line) => line.brand.toLowerCase() === query && line.prodOptions)
        .filter((item) =>
            item.prodOptions.filter((item_in) =>
                item_in.value.toLowerCase().includes("ssd")))
        .map((line) => {
            return {
                name: line.name,
                brand: line.brand,
                salePriceU: line.salePriceU,
                diagonal: line.prodOptions.find(item => item.name === "Диагональ экрана")?.value,
                resolution: line.prodOptions.find(item => item.name === "Разрешение экрана")?.value,
                vROM: line.prodOptions.find(item => item.name === "Объем оперативной памяти (Гб)")?.value,
                vSSD: line.prodOptions.find(item => item.name === "Объем накопителя SSD")?.value,
                matrix: line.prodOptions.find(item => item.name === "Тип матрицы")?.value
            }
        })
        .sort((a, b) => a.salePriceU < b.salePriceU? +1: -1)

    console.table(res)
    let fields = 'name;brand;salePriceU;diagonal;resolution;vROM;vSSD;matrix'.split(';')
    write_to_csv("./laptops.csv", res, fields)
}


url = "http://files-pcoding.1gb.ru/json?filename=wb_laptops_popular.json"
let query = "lenovo"
processData(url, query.toLowerCase())
