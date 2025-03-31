const _ = require("lodash")
const colors = require("./json/colors.json")

_
    .map(colors, (color) => {
        const colorKey = _.keys(color)[0]
        const rgb = _.values(color)[0].slice(0, 3)

        return {
            color: colorKey,
            rgb: rgb
        }
    })
    .sort()
    // .reverse()
    // .sort((a, b) => {
    //     const colorA = a.color.toLowerCase()
    //     const colorB = b.color.toLowerCase()
    //     return (colorB < colorA) - (colorA < colorB)
    // })
    .forEach(x => console.log(JSON.stringify(x)))
    