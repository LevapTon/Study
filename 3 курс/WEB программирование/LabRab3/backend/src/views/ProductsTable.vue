<script setup>
</script>

<template>
  <main class="container">
    <h1>Список товаров</h1>
    <div class="d-flex justify-content-between mb-3">
        <div>
            <router-link class="btn btn-warning" to="/products">Перейти к карточкам товаров</router-link>
        </div>
        <div v-if="checkAccess">
            <router-link class="btn btn-info" to="/add_product">Добавить товар</router-link>
        </div>
    </div>

    <div>
        <table class="table table-striped table-bordered">
            <thead>
                <tr>
                    <th>№</th>
                    <th>Наименование</th>
                    <th>Описание</th>
                    <th>Стоимость</th>
                    <th>Купить</th>
                </tr>
            </thead>
            <tbody>
                <tr v-for="product in products" :key="product.id">
                    <td>{{ product.id + 1 }}</td>
                    <td>{{ product.name }}</td>
                    <td>{{ product.description }}</td>
                    <td><strong>{{ product.price }} ₽</strong></td>
                    <td><button class="btn btn-warning" title="В корзину" onclick="alert('Товар добавлен в корзину')">
                        <i class="bi bi-cart"></i> <span class="d-none d-md-inline">В корзину</span>
                    </button></td>
                </tr>
            </tbody>
        </table>
    </div>
  </main>
</template>

<script>
import axios from "axios"

export default {
    name: "products",

    data() {
        return {
            products: {
                ID: 0,
                Name: "", 
                Description: "",
                Price: "",
                Image: ""
            },
        }
    },

    computed: { 
        checkAccess() {
            const store = JSON.parse(localStorage.getItem('session-name'));
            return store.access;
        }
    },

    created() {
        this.loadProducts();
    },

    methods: {
        loadProducts() {
            axios.post(`/products_table`).then(response => {
                this.products = response.data.products
            })
        },
    }
}
</script>
