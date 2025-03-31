package main

import (
	"encoding/json"
	"fmt"
	"net/http"
	"os"
	"strconv"

	"github.com/gorilla/sessions"
	"github.com/labstack/echo/v4"
)

type Products struct {
	ID          int    `json:"id"`
	Name        string `json:"name"`
	Description string `json:"description"`
	Price       int    `json:"price"`
	Image       string `json:"image"`
}

type Employees struct {
	ID          int    `json:"id"`
	Name        string `json:"name"`
	Post        string `json:"post"`
	Description string `json:"description"`
	Number      string `json:"number"`
	Mail        string `json:"mail"`
	Photo       string `json:"photo"`
	Path        string `json:"path"`
}

type Accounts struct {
	ID       int    `json:"id"`
	Name     string `json:"name"`
	Password string `json:"password"`
	Access   bool   `json:"access"`
	Cart     []CartItem
}

type CartItem struct {
	ProductID   int    `json:"productID"`
	ProductName string `json:"productName"`
	Image       string `json:"image"`
	Quantity    int    `json:"quantity"`
	Price       int    `json:"price"`
}

var products []Products
var productsPath = "private/products.json"
var employees []Employees
var employeesPath = "private/employees.json"
var accounts []Accounts
var accountsPath = "private/accounts.json"
var store = sessions.NewCookieStore([]byte("QWERTY"))

func main() {
	loadAccounts()
	getProductsList()
	getEmployeesList()
	e := echo.New()

	e.Static("/public", "public")
	e.Static("/assets", "public/assets")

	e.POST("/products/:page", pushProductsList)
	e.POST("/products_table", pushProductsForTableList)
	e.POST("/about", pushEmloyeesList)
	e.POST("/login", loginAccount)
	e.POST("/registration", registerAccount)
	e.POST("/add_product", addProduct)
	e.POST("/add_to_cart", addToCart)
	e.POST("/get_cart", getCart)

	e.DELETE("/products/:id", checkAccess(true)(deleteProduct))
	e.DELETE("/delete_item/:productID", deleteCartItem)

	e.PUT("/products/:id", checkAccess(true)(updateProduct))

	e.GET("/testget", func(c echo.Context) error {
		name := c.QueryParam("name")

		return c.String(http.StatusOK, "Добрый день, "+name)
	})

	e.POST("/testpost", func(c echo.Context) error {
		json_map := make(map[string]interface{})
		err := json.NewDecoder(c.Request().Body).Decode(&json_map)
		if err != nil {
			return err
		}

		fmt.Println(json_map)
		name := json_map["name"].(string)
		v := map[string]interface{}{
			"response": "Добрый день, " + name,
		}

		return c.JSON(http.StatusOK, v)
	})

	e.GET("*", func(c echo.Context) error {
		return c.File("index.html")
	})

	e.Logger.Fatal(e.Start(":1323"))
}

func init() {
	store.Options = &sessions.Options{
		Path:     "/",
		MaxAge:   604800,
		HttpOnly: true,
	}
}

// загрузка товаров
func getProductsList() {
	productsList, err := os.ReadFile(productsPath)
	if err != nil {
		panic(err)
	}
	if err := json.Unmarshal(productsList, &products); err != nil {
		panic(err)
	}
}

// отправка товаров на страницу с карточками
func pushProductsList(c echo.Context) error {
	page, _ := strconv.Atoi(c.Param("page"))
	itemsOnPage := 3
	start := (page - 1) * itemsOnPage
	end := start + itemsOnPage
	if end > len(products) {
		end = len(products)
	}
	if start < 0 || start > end || start >= len(products) {
		return echo.NewHTTPError(http.StatusBadRequest, "Неверный номер страницы")
	}

	return c.JSON(http.StatusOK, map[string]interface{}{
		"products": products[start:end],
		"total":    len(products),
	})
}

// отправка товаров на страницу с таблицей
func pushProductsForTableList(c echo.Context) error {
	return c.JSON(http.StatusOK, map[string]interface{}{
		"products": products,
	})
}

// загрузка сотрудников
func getEmployeesList() {
	employeesList, err := os.ReadFile(employeesPath)
	if err != nil {
		panic(err)
	}
	if err := json.Unmarshal(employeesList, &employees); err != nil {
		panic(err)
	}
}

// отправка сотрудников на страницу
func pushEmloyeesList(c echo.Context) error {
	return c.JSON(http.StatusOK, map[string]interface{}{
		"employees": employees,
	})
}

// загрузка аккаунтов
func loadAccounts() {
	data, _ := os.ReadFile(accountsPath)
	if err := json.Unmarshal(data, &accounts); err != nil {
		panic(err)
	}
}

// вход в аккаунт
func loginAccount(c echo.Context) error {
	var loginAttempt Accounts
	if err := c.Bind(&loginAttempt); err != nil {
		return echo.NewHTTPError(http.StatusBadRequest, "Неверные данные входа")
	}
	for _, existingAccount := range accounts {
		if existingAccount.Name == loginAttempt.Name {
			err := existingAccount.Password == loginAttempt.Password
			if err == true {
				session, _ := store.Get(c.Request(), "session-name")
				session.Values["name"] = existingAccount.Name
				session.Values["access"] = existingAccount.Access
				session.Save(c.Request(), c.Response())

				return c.JSON(http.StatusOK, map[string]interface{}{
					"message": "Вход выполнен успешно",
					"name":    existingAccount.Name,
					"access":  existingAccount.Access,
				})
			} else {
				return echo.NewHTTPError(http.StatusUnauthorized, "Неверные логин или пароль")
			}
		}
	}

	return echo.NewHTTPError(http.StatusUnauthorized, "Неверные логин или пароль")
}

// регистрация аккаунта
func registerAccount(c echo.Context) error {
	var newAccount Accounts
	if err := c.Bind(&newAccount); err != nil {
		return echo.NewHTTPError(http.StatusBadRequest, "Неверные данные пользователя")
	}
	newAccount.ID = getNewAccountID()
	for _, existingAccount := range accounts {
		if existingAccount.Name == newAccount.Name {
			return echo.NewHTTPError(http.StatusBadRequest, "Такой пользователь уже существует")
		}
	}
	newAccount.Access = false
	newAccount.Cart = []CartItem{}
	accounts = append(accounts, newAccount)
	saveAccounts()
	return c.JSON(http.StatusOK, newAccount)
}

// получение нового ID аккаунта
func getNewAccountID() int {
	if len(accounts) == 0 {
		return 1
	}
	return accounts[len(accounts)-1].ID + 1
}

// сохранение аккаунтов
func saveAccounts() {
	data, err := json.Marshal(accounts)
	if err != nil {
		panic(err)
	}

	err = os.WriteFile(accountsPath, data, 0644)
	if err != nil {
		panic(err)
	}
}

// добавление продукта
func addProduct(c echo.Context) error {
	var product Products
	if err := c.Bind(&product); err != nil {
		return echo.NewHTTPError(http.StatusBadRequest, "Неверные данные продукта")
	}
	product.ID = getNewProductID()
	products = append(products, product)
	saveProducts()
	return c.JSON(http.StatusOK, product)
}

// получение нового ID товара
func getNewProductID() int {
	if len(products) == 0 {
		return 1
	}
	return products[len(products)-1].ID + 1
}

// сохранение товара
func saveProducts() {
	data, err := json.Marshal(products)
	if err != nil {
		panic(err)
	}

	err = os.WriteFile(productsPath, data, 0644)
	if err != nil {
		panic(err)
	}
}

// удаление товара
func deleteProduct(c echo.Context) error {
	id, err := strconv.Atoi(c.Param("id"))
	if err != nil {
		return echo.NewHTTPError(http.StatusBadRequest, "Неверный ID продукта")
	}
	var found bool
	for i, product := range products {
		if product.ID == id {
			products = append(products[:i], products[i+1:]...)
			found = true
			break
		}
	}
	if !found {
		return echo.NewHTTPError(http.StatusNotFound, "Продукт не сущетсвует")
	}
	saveProducts()

	return c.NoContent(http.StatusNoContent)
}

// проверка доступа
func checkAccess(access bool) echo.MiddlewareFunc {
	return func(next echo.HandlerFunc) echo.HandlerFunc {
		return func(c echo.Context) error {
			session, _ := store.Get(c.Request(), "session-name")
			userAccess, ok := session.Values["access"]
			if !ok || userAccess != access {
				return echo.NewHTTPError(http.StatusForbidden, "Отказано в доступе")
			}
			return next(c)
		}
	}
}

// добавление товара в корзину
func addToCart(c echo.Context) error {
	session, _ := store.Get(c.Request(), "session-name")
	name, ok := session.Values["name"].(string)
	if !ok {
		return echo.NewHTTPError(http.StatusUnauthorized, "Ошибка авторизации")
	}

	var cartItem CartItem
	if err := c.Bind(&cartItem); err != nil {
		return echo.NewHTTPError(http.StatusBadRequest, "Неверные данные корзины")
	}

	for i, account := range accounts {
		if account.Name == name {
			for _, product := range products {
				if product.ID == cartItem.ProductID {
					cartItem.ProductName = product.Name
					cartItem.Image = product.Image
					cartItem.Price = product.Price
					break
				}
			}
			accounts[i].Cart = append(accounts[i].Cart, cartItem)
			saveAccounts()
			return c.JSON(http.StatusOK, account)
		}
	}

	return echo.NewHTTPError(http.StatusNotFound, "Пользователь не найден")
}

// загрузка корзины пользователя
func getCart(c echo.Context) error {
	session, _ := store.Get(c.Request(), "session-name")
	name, ok := session.Values["name"].(string)
	if !ok {
		return echo.NewHTTPError(http.StatusUnauthorized, "Пользователь не авторизован")
	}

	for _, account := range accounts {
		if account.Name == name {
			for _, cartItem := range account.Cart {
				for _, product := range products {
					if cartItem.ProductID == product.ID {
						cartItem.ProductName = product.Name
						cartItem.Image = product.Image
						cartItem.Price = product.Price
						break
					}
				}
			}
			return c.JSON(http.StatusOK, account.Cart)
		}
	}

	return echo.NewHTTPError(http.StatusNotFound, "Пользователь не найден")
}

// удаление товара из корзины
func deleteCartItem(c echo.Context) error {
	session, _ := store.Get(c.Request(), "session-name")
	name, ok := session.Values["name"].(string)
	if !ok {
		return echo.NewHTTPError(http.StatusUnauthorized, "Пользователь не авторизован")
	}
	for i, account := range accounts {
		if account.Name == name {
			id, err := strconv.Atoi(c.Param("productID"))
			if err != nil {
				return echo.NewHTTPError(http.StatusBadRequest, "Неверный ID продукта")
			}
			var found bool
			for j, item := range account.Cart {
				if item.ProductID == id {
					accounts[i].Cart = append(account.Cart[:j], account.Cart[j+1:]...)
					found = true
					saveAccounts()
					break
				}
			}
			if !found {
				return echo.NewHTTPError(http.StatusNotFound, "Продукт не найден")
			}
			break
		}
	}

	return c.NoContent(http.StatusNoContent)
}

// изменение товара
func updateProduct(c echo.Context) error {
	id, err := strconv.Atoi(c.Param("id"))
	if err != nil {
		return echo.NewHTTPError(http.StatusBadRequest, "Неверный ID продукта")
	}

	var updatedProduct Products
	if err := c.Bind(&updatedProduct); err != nil {
		return echo.NewHTTPError(http.StatusBadRequest, "Неверные данные продукта")
	}

	var found bool
	for i, product := range products {
		if product.ID == id {
			updatedProduct.ID = id
			products[i] = updatedProduct
			found = true
			break
		}
	}

	if !found {
		return echo.NewHTTPError(http.StatusNotFound, "Продукт не найден")
	}

	saveProducts()

	return c.JSON(http.StatusOK, updatedProduct)
}
