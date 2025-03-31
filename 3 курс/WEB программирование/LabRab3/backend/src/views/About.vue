<script setup>
</script>

<template>
    <main class="container">
        <h1>Магазин обуви "Ромашка"</h1>
        <p>Мы работаем с 2014 года. За это время мы открыли множество торговых точек и обслужили тысячи довольных клиентов!</p>
        <h1>Люди, которые стараются ради вас!</h1>

        <div class="row">
            <div class="col-md-4" v-for="person in employees" :key="person.id">
                <div class="card shadow rounded my-3">
                    <router-link class="text-decoration-none text-dark" :to="person.path">
                        <div class="card-body">
                            <div><img :src="person.photo" class="avatar"></div>
                            <div class="avatar-post" v-if="person.post === 'Генеральный директор'"><i class="bi-gem"></i> {{ person.post }}</div>
                            <div class="avatar-post" v-else-if="person.post === 'Маркетолог'"><i class="bi-cup-hot"></i> {{ person.post }}</div>
                            <div class="avatar-post" v-else-if="person.post === 'Рабочий'"><i class="bi-hammer"></i> {{ person.post }}</div>
                            <div class="avatar-text">{{ person.description }}</div>
                            <div class="avatar-text"><a href="mailto:mail@htmlacademy.ru&subject=вопрос">Моя почта</a></div>
                        </div>
                    </router-link>
                </div>
            </div>
        </div>
    </main>
</template>

<script>
import axios from "axios"

export default {
    name: "employees",

    data() {
        return {
            employees: {
                ID: 0,
                Name: "", 
                Post: "",
                Description: "",
                Number: "",
                Mail: "",
                Photo: "",
                Path: ""
            },
        }
    },

    computed: { 
    },

    created() {
        this.loadEmployees();
    },

    methods: {
        loadEmployees() {
            axios.post(`/about`).then(response => {
                this.employees = response.data.employees
            })
        },
    }
}
</script>