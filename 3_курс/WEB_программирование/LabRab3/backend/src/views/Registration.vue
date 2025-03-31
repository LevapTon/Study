<template>
  <div class="container d-flex justify-content-center align-items-center">
    <div class="card my-3">
      <div class="card-body">
        <h2 class="card-title">Регистрация</h2>
        <form @submit.prevent="register">
          <div class="mb-3">
            <label for="registerName" class="form-label">Имя пользователя:</label>
            <input type="text" id="registerName" v-model="name" class="form-control" required>
          </div>
          <div class="mb-3">
            <label for="registerPassword" class="form-label">Пароль:</label>
            <input type="password" id="registerPassword" v-model="password" class="form-control" required>
          </div>
          <div class="row">
            <button type="submit" class="btn btn-info col mx-3">Зарегистрироваться</button>
          </div>
        </form>
      </div>
    </div>
  </div>
</template>

<script>
import Swal from 'sweetalert2';

export default {
  data() {
    return {
      name: '',
      password: ''
    };
  },
  methods: {
    register() {
      const registerData = {
        name: this.name,
        password: this.password
      };

      fetch('http://localhost:1323/registration', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(registerData),
      })
      .then(response => {
        if (!response.ok) {
          throw new Error('Ошибка регистрации');
        }
        return response.json();
      })
      .then(data => {
        console.log('Регистрация выполнена успешно', data);
        Swal.fire({
          icon: 'success',
          title: 'Регистрация выполнена успешно',
          showConfirmButton: false,
          timer: 1500,
        });
        this.$router.push('/login');
      })
      .catch(error => {
        console.error('Ошибка регистрации:', error.message);
        Swal.fire({
          icon: 'error',
          title: 'Такой пользователь уже есть, измените "Имя пользователя"',
          text: error.message,
        });
      });
    },
  }
};
</script>
