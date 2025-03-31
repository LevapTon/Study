const fs = require("fs");

const host = "https://search.wb.ru/";
const way = "exactmatch/ru/male/v5/search";

const sortTypes = [
  { type: "priceBasic", direction: "asc" },
  { type: "priceBasic", direction: "desc" },
  { type: "feedbacks", direction: "asc" },
  { type: "feedbacks", direction: "desc" },
  { type: "reviewRating", direction: "asc" },
  { type: "reviewRating", direction: "desc" },
];

let config = require("./json/config.json")

const selectedSort = sortTypes[config['sortType']];

let params = require("./json/params.json");
params.sort = selectedSort.type;
params.sortDirection = selectedSort.direction;
console.log(params.sortDirection)

const searchQuery = config['query'];
params.query = searchQuery;

const pagesCount = config['pages'];

let allProducts = [];

(async () => {
  for (let page = 1; page <= pagesCount; page++) {
    params.page = page;

    let query = Object.keys(params)
      .map((key) => `${key}=${encodeURIComponent(params[key])}`)
      .join("&");
    let url = `${host}${way}?${query}`;
    let headers = require("./json/headers.json");

    let resp = await fetch(url, { headers });
    let json = await resp.json();
    let products = json.data.products.map((product) => {
      return {
        brand: product.brand,
        colors: product.colors.map((color) => color.name),
        name: product.name,
        priceBasic: product.sizes[0].price.basic,
        priceProduct: product.sizes[0].price.product,
        reviewRating: product.reviewRating,
        feedbacks: product.feedbacks,
      };
    });

    allProducts = allProducts.concat(products);
  }

  // Сортировка всех продуктов
  allProducts.sort((a, b) => {
    let fieldA = a[selectedSort.type];
    let fieldB = b[selectedSort.type];
    
    if (typeof fieldA === "string" && typeof fieldB === "string") {
      fieldA = fieldA.toLowerCase();
      fieldB = fieldB.toLowerCase();
    }

    let d = (selectedSort.direction === "asc"?-1:+1);
    return d * (fieldA < fieldB ? +1: -1);
  });

  let output = { products: allProducts };

  fs.writeFileSync("./json/products.json", JSON.stringify(output, null, 2));
})();