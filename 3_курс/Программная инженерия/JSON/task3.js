// const _ = require("lodash")
// const colors = require("./json/colors.json")

console.table(
    require("./json/colors.json")
        .map(clr => Object.keys(clr)[0])
        .filter(clr => clr.length < 6)
        .sort()
)
// _
//     .map(colors, (color) => {
//         return _.keys(color)
//     })
//     .filter(color => {
//         return _.size(color) < 6
//     })
//     .sort()
//     .forEach(x => console.log(x))
    