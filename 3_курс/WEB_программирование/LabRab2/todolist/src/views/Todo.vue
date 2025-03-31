<script setup>
// Эта часть выполняется при первоначальной загрузке компонента
</script>
<template>
  <div class="container bg-secondary text-white">
    <div class="d-flex p-5 justify-content-between">
      <div style="width:45%">
        <h1>Мои заметки</h1>
        <ul class="list-unstyled" v-if="notes.length > 0">
          <li v-for="note in notes" :key="note.id" @click="selectNote(note)">
            <div class="p-1 d-flex justify-content-between" >
              <span v-if="!note.editingTitle">{{ note.title }}</span>
              <input v-else v-model="note.title" @blur="saveNoteTitle(note)" @keyup.enter="saveNoteTitle(note)" />
              <div class="d-flex justify-content-end">
                <button @click.stop="editNoteTitle(note)" :class="{ 'btn-warning': !note.editingTitle, 'btn-success': note.editingTitle}">
                  <i :class="{ 'bi bi-pencil': !note.editingTitle, 'bi bi-check': note.editingTitle }"></i>
                </button>
                <button @click.stop="deleteNote(note)" class="btn-danger mx-1">
                  <i class="bi bi-trash"></i>
                </button>
              </div>
            </div>
          </li>
        </ul>
        <p v-else>Нет заметок...</p>
        
        <button @click="addNote" class="btn btn-primary btn-block" style="width: 100%;">+ Добавить заметку</button>
      </div>
      <div style="width:45%">
        <h1 v-if="selectedNote">Заметка: {{ selectedNote.title }}</h1>
        <h1 v-else>Выберете заметку...</h1>
        <ul class="list-unstyled" v-if="selectedNote">
          <li v-for="(todo, index) in selectedNote.todos" :key="index">
            <div class="p-1 d-flex justify-content-between">
              <div class="d-flex justify-content-start">
                <div class="d-flex align-items-center">
                  <input class="form-check-input mx-2" type="checkbox" v-model="todo.completed" id="checkbox{{ index }}" />
                  <div class="test">
                    <span  v-if="!todo.editing">{{ todo.text }}</span>
                    <input v-else v-model="todo.text" @blur="saveTodoText(index)" @keyup.enter="saveTodoText(index)" />
                  </div>
                </div>
              </div>
              <div class="d-flex justify-content-end">
                <button @click.stop="editTodo(index)" :class="{ 'btn-warning': !selectedNote.todos[index].editing, 'btn-success': selectedNote.todos[index].editing}">
                  <i :class="{ 'bi bi-pencil': !selectedNote.todos[index].editing, 'bi bi-check': selectedNote.todos[index].editing }"></i>
                </button>
                <button @click.stop="deleteTodo(index)" class="btn-danger mx-1">
                  <i class="bi bi-trash"></i>
                </button>
            </div>
            </div>
          </li>
        </ul>
        <button v-if="selectedNote" @click="addTodo" class="btn btn-primary btn-block" style="width: 100%;">+ Добавить задачу</button>
      </div>
    </div>
  </div>
</template>


<script>
export default {
  data() {
    return {
      notes: [
        { id: 1, title: "Заметка 1", todos: [{ text: "Задача 1" }], editingTitle: false, originalTitle: "" },
        { id: 2, title: "Заметка 2", todos: [], editingTitle: false, originalTitle: "" }
      ],
      selectedNote: null,
      newNoteTitle: "",
      newTodoText: "",
    };
  },

  methods: {
    createNote() {
      if (this.newNoteTitle.trim() !== "") {
        const newNote = {
          id: this.notes.length + 1,
          title: this.newNoteTitle,
          todos: []
        };
        this.notes.push(newNote);
        this.newNoteTitle = "";
      }
    },
    selectNote(note) {
      this.selectedNote = note;
    },

    deleteNote(note) {
      const confirmDelete = confirm("Вы уверены, что хотите удалить эту заметку?");
      if (confirmDelete) {
        const index = this.notes.indexOf(note);
        this.notes.splice(index, 1);
        this.selectedNote = null;
      }
    },

    addTodo() {
      if (this.selectedNote) {
        this.selectedNote.todos.push({ text: "Новая задача" });
      }
    },

    editTodo(index) {
      if (this.selectedNote) {
        this.selectedNote.todos[index].editing = !this.selectedNote.todos[index].editing;
      }
    },

    saveTodoText(index) {
      if (this.selectedNote) {
        this.selectedNote.todos[index].editing = false;
      }
    },

    deleteTodo(index) {
      if (this.selectedNote) {
        const confirmDelete = confirm("Вы уверены, что хотите удалить эту задачу?");
        if (confirmDelete) {
          this.selectedNote.todos.splice(index, 1);
        }
      }
    },

    addNote() {
      const newNote = {
        id: this.notes.length + 1,
        title: "Новая заметка",
        todos: []
      };
      this.notes.push(newNote);
    },
    editNoteTitle(note) {
      note.editingTitle = !note.editingTitle;
      if (!note.editingTitle) {
        if (note.title.trim() === "") {
          note.title = note.originalTitle;
        }
      } else {
        note.originalTitle = note.title;
      }
    },

    saveNoteTitle(note) {
      note.editingTitle = false;
    },
  }
};
</script>