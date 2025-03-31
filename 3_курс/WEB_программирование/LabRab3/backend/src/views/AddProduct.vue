<template>
    <div class="container d-flex justify-content-center my-3">
        <div class="card">
            <div class="card-body">
                <h2>Добавление нового товара</h2>
                <div>
                    <form @submit.prevent="add">
                        <div class="mb-3">
                            <label for="prodName" class="form-label">Наименование:</label>
                            <input type="text" id="prodName" v-model="name" class="form-control" required>
                        </div>
                        <div class="mb-3">
                            <label for="prodDesc" class="form-label">Описание:</label>
                            <input type="text" id="prodDesc" v-model="description" class="form-control" required>
                        </div>
                        <div class="mb-3">
                            <label for="prodCost" class="form-label">Стоимость (руб.):</label>
                            <input type="text" id="prodCost" v-model="price" class="form-control" required>
                        </div>
                        <div class="mb-3">
                            <label for="prodImg" class="form-label">Изображение (URL):</label>
                            <input type="text" class="form-control" id="prodImg" v-model="image" required>
                        </div>
                        <div class="row">
                            <button type="submit" class="btn btn-primary col mx-3">Добавить</button>
                        </div>
                    </form>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
export default {
    data() {
        return {
            product: {
                id: 0,
                name: "", 
                description: "",
                price: 0,
                image: "empty"
            },
        }
    },

    computed: { 
    },

    created() {
        const store = JSON.parse(localStorage.getItem('session-name'));
        if (store && store.access === false) {
            this.$router.push("/")
        }
    },

    methods: {
        add() {
            const prodData = {
                Name: this.name,
                Description: this.description,
                Price: +this.price,
                Image: this.image
            };
            fetch('http://localhost:1323/add_product', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(prodData),
      })
        .then(response => {
          if (!response.ok) {
            throw new Error('Ошибка загрузки товара');
          }
          return response.json();
        })
        .then(data => {
          console.log('Успешное добавление товара', data);
          this.$router.push('/products/');
        })

        .catch(error => {
          console.error('Ошибка добавления товара:', error.message);

        });
        }
        
        
    }
}
</script>
