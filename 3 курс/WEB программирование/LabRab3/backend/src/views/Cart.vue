<template>
    <div class="container">
      <h1>Корзина</h1>
        <div class="container d-flex justify-content-center my-3" v-if="!checkCart">
            <h2>Ваша корзина пуста</h2>
        </div>
        <div v-else>
            <table class="table table-striped table-bordered">
                <thead>
                    <tr>
                        <th>Изображение</th>
                        <th>Наименование</th>
                        <th>Стоимость</th>
                        <th>Количество</th>
                        <th>Сумма</th>
                        <th>Удалить</th>
                    </tr>
                </thead>
                <tbody>
                    <tr v-for="item in cartItems" :key="item.productID">
                        <td class="d-flex justify-content-center"><img :src="item.image" class="product-image"></td>
                        <td class="text-center align-middle">{{ item.productName }}</td>
                        <td class="text-center align-middle">{{ item.price }} ₽</td>
                        <td class="text-center align-middle">{{ item.quantity }}</td>
                        <td class="text-center align-middle"><strong>{{ item.price * item.quantity }} ₽</strong></td>
                        <td class="text-center align-middle"><button class="btn btn-danger" @click="deleteItem(item)"><i class="bi bi-trash"></i></button></td>
                    </tr>
                </tbody>
            </table>
            <div class="mb-3 d-flex justify-content-end">
                <span>Итого: <strong>{{ totalSum }} ₽</strong></span>
            </div>
        </div>
    </div>
  </template>
  
  <script>
  import axios from 'axios';
  
  export default {
    data() {
      return {
        cartItems: [],
        totalSum: 0
      };
    },
    mounted() {
      this.loadCart()
    },
    computed: {
        checkCart() {
            if (this.cartItems.length == 0) {
                return false
            } else {
                return true
            }
        },
    },
    methods: {
      loadCart() {
        axios.post('/get_cart')
          .then(response => {
            this.cartItems = response.data;
            this.getSum()
          })
          .catch(error => {
            console.error('Ошибка загрузки корзины:', error);
          });
      },
      getSum() {
        const nums = this.cartItems.map(item => {
            return {
                Price: item.price,
                Quantity: item.quantity
            }
        })
        for (var i = 0; i < nums.length; i++) {
            console.log(nums[i])
            this.totalSum += nums[i].Price * nums[i].Quantity
        }
      },
      async deleteItem(item) {
            try {
                await axios.delete(`/delete_item/${item.productID}`);
                    this.totalSum = 0
                    this.loadCart()
            }
            catch (error) {
                console.error('Ошибка удаления товара из корзины:', error);
            }
        }
    },
  };
  </script>
  