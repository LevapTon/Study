<script setup>
</script>

<template>
  <main class="container">
    <h1>Список товаров</h1>
    <div class="d-flex justify-content-between">
        <div><router-link class="btn btn-warning" to="/products_table">Перейти к таблице товаров</router-link></div>
        <div>
             <nav>
                <ul class="pagination">
                    <li class="page-item">
                    <button class="page-link" @click="prevPage" :disabled="page === 1">Предыдущая</button>
                    </li>
                    <li class="page-item" :class="{ active: page === pageNumber }" v-for="pageNumber in pages" :key="pageNumber">
                    <a class="page-link" @click="goToPage(pageNumber)">{{ pageNumber }}</a>
                    </li>
                    <li class="page-item">
                    <button class="page-link" @click="nextPage" :disabled="page === pages">Следующая</button>
                    </li>
                </ul>
            </nav>
        </div>
        <div v-if="checkAccess">
            <router-link class="btn btn-warning" to="/add_product">Добавить товар</router-link>
        </div>
    </div>

    <div class="card shadow rounded my-3" v-for="product in products" :key="product.id">
        <div class="card-body">
            <div class="row">
                    <div class="col-md-2">
                        <img :src="product.image" class="product-image">
                    </div>
                <div class="col-md-10 d-flex flex-column">
                    <div class="product-name">{{ product.name }}</div>
                    <div class="product-description">{{ product.description }}</div>
                    <div class="flex-grow-1"></div>
                    <div class="d-flex justify-content-end" v-if="checkAccess">
                        <div>
                            <button class="btn btn-secondary mx-2" @click="editProduct(product)" on-focus="editCard">Изменить</button>
                            <button class="btn btn-danger" @click="deleteProduct(product)">Удалить</button>
                        </div>
                    </div>
                    <div class="d-flex justify-content-end mt-2">
                        <div class="product-price">Цена: <strong>{{ product.price }} ₽</strong></div>
                        <button 
                            class="btn btn-warning" 
                            @click="addToCart(product)"
                            v-if="!checkAccess">
                            <i class="bi bi-cart"></i> <span class="d-none d-md-inline">В корзину</span>
                        </button>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <div class="container d-flex justify-content-center my-3" v-if="checkAccess && edit">
        <div class="card shadow rounded my-3">
          <div class="card-body">
            <h2>Редактирование товара</h2>
            <form @submit.prevent="saveProduct">
              <div class="mb-3">
                <label for="name" class="form-label">Наименование</label>
                <input type="text" class="form-control" id="name" v-model="productsData.name" required>
              </div>
              <div class="mb-3">
                <label for="description" class="form-label">Описание</label>
                <textarea class="form-control" id="description" v-model="productsData.description" required></textarea>
              </div>
              <div class="mb-3">
                <label for="image" class="form-label">Изображение (URL)</label>
                <input type="text" class="form-control" id="image" v-model="productsData.image" required>
              </div>
              <div class="mb-3">
                <label for="price" class="form-label">Цена</label>
                <input type="text" class="form-control" id="price" v-model="productsData.price" required>
              </div>
              <div class="row">
                <button type="submit" class="btn btn-primary col mx-3">Сохранить</button>
              </div>
            </form>
          </div>
        </div>
    </div>
  </main>
</template>

<script>
import axios from "axios"
import Swal from 'sweetalert2';

export default {
    props: ['initialPage'],
    name: "products",

    data() {
        return {
            page: parseInt(this.initialPage) || 1,
            count: 0,
            itemsPerPage: 3,
            edit: false,
            cart: [],
            products: [],
            productsData: {
                id: 0,
                name: "", 
                description: "",
                price: 0,
                image: ""
            },
        }
    },

    computed: { 
        pages() {
            return Math.ceil(this.count / this.itemsPerPage)
        },
        checkAccess() {
            const store = JSON.parse(localStorage.getItem('session-name'));
            return store.access;
        }
    },

    created() {
        const pageParam = this.$route.params.page;
        this.page = parseInt(pageParam) || 1;
        this.loadProducts();
        this.loadCart()
    },

    methods: {
        loadProducts() {
            axios.post(`/products/${this.page}`).then(response => {
                this.products = response.data.products
                this.count = response.data.total
            })
        },

        beforeRouteUpdate(to, from, next) {
            this.page = parseInt(to.params.page) || 1;
            next();
        },

        goToPage(pageNumber) {
            if (pageNumber >= 1 && pageNumber <= this.pages) {
                this.page = pageNumber
                this.loadProducts()
                this.$router.push({ name: 'products', params: { page: pageNumber } });
            }
        },

        prevPage() {
            if (this.page > 1) {
                this.page--;
                this.loadProducts();
                this.$router.push({ name: 'products', params: { page: this.page } });
            }
        },

        nextPage() {
            if (this.page < this.pages) {
                this.page++;
                this.loadProducts();
                this.$router.push({ name: 'products', params: { page: this.page } });
            }
        },
        async deleteProduct(product) {
            try {
                    await axios.delete(`/products/${product.id}`);
                    this.loadProducts();
                    if (this.products.length <= 1 && this.page > 1) {
                        this.page--;
                        this.loadProducts();
                        this.$router.push({ name: 'products', params: { page: this.page } });
                    }
            } catch (error) {
                console.error('Ошибка удаления товара:', error);
            }
        },
        editProduct(product) {
            this.productsData = { ...product }
            this.edit = true
        },
        addToCart(product) {
            const cartItem = {
                productID: product.id,
                quantity: 1,
                productName: product.name,
                price: product.price,
            };

            axios.post('/add_to_cart', cartItem)
                .then(() => {
                    Swal.fire({
                        icon: 'success',
                        title: 'Товар добавлен в корзину',
                        showConfirmButton: false,
                        timer: 1500
                    });
                })
                .catch(error => {
                    console.error('Ошибка добавления товара в корзину:', error);
                    Swal.fire({
                        icon: 'error',
                        title: 'Ошибка',
                        text: 'Не удалось добавить товар в корзину'
                    });
                });
        },
        loadCart() {
            axios.get('/get_cart')
                .then(response => {
                    this.cart = response.data;
                })
                .catch(error => {
                    console.error('Ошибка загрузки корзины:', error);
            });
        },

        saveProduct() {
            try {
                this.productsData.price = parseInt(this.productsData.price)
                axios.put(`/products/${this.productsData.id}`, this.productsData).then(() => {
                this.edit = false
                this.loadProducts()
                })
                this.productsData = {
                    name: '',
                    description: '',
                    image: '',
                    price: 0,
                }
            } 
            catch (error) {
                console.error('Ошибка обновления товара:', error)
            }
            
        },
    }
}
</script>
