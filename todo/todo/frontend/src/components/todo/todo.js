import React, { Component } from 'react'

import './todo.css';

import TodoField from '../todo-field';

export default class Todo extends Component {

  state = {
    inputTask: '',
    tasks: []
  };

  getTasks = () => {

    fetch('http://localhost:8000/api/task/', {
      method: 'GET',
      headers: {
        "Content-type": "application/json",
        "Authorization": `Token ${this.props.token}`
      }
    })
    .then((res) => {
      return res.json();
    })
    .then((body) => {
      this.setState(() => {
        return {
          tasks: [ ...body ]
        };
      });
    })
    .catch((err) => {
      console.log(err.message);
    });

  };

  createTask = () => {
    
    fetch('http://localhost:8000/api/task/', {
      method: 'POST',
      headers: {
        "Content-type": "application/json",
        "Authorization": `Token ${this.props.token}`
      },
      body: JSON.stringify({
        name: this.state.inputTask,
        description: '',
        difficult: 2
      })
    })
    .then((res) => {
      return res.json();
    })
    .then((body) => {
      console.log(body);
      this.setState(({ tasks, inputTask }) => {
        return {
          inputTask: '',
          tasks: [...tasks, body]
        };
      });
    })
    .catch((err) => {
      console.log(2, err.message);
    });

  };

  deleteTask = (id) => {
    fetch('http://localhost:8000/api/task/', {
      method: 'DELETE',
      headers: {
        "Content-type": "application/json",
        "Authorization": `Token ${this.props.token}`
      },
      body: JSON.stringify({
        id: id
      })
    })
    .then((res) => {
      return res.json();
    })
    .then((body) => {
      console.log(body);
      this.setState(({ tasks }) => {
        let idDel = tasks.findIndex((val) => {
          return val.id === id;
        });
        const newTasks = [...tasks.slice(0, idDel), ...tasks.slice(idDel + 1)]
        return {
          tasks: newTasks
        };
      });
    })
    .catch((err) => {
      console.log(err.message);
    });
  }

  onChangeTask = (event)  => {
    this.setState({ inputTask: event.target.value });
    console.log(this.state.inputTask);
  };

  componentWillMount() {
    this.getTasks();
  };

  render() {  
    return (
      <div className="todo">
        <h3 className="todo-title">Todo App</h3>
        <ul className="list-group todo-list">
          {this.state.tasks.map(({ name, id }) => {return <TodoField
            key={ id }
            name={ name }
            num={ id }
            onDelete={ this.deleteTask }
            />
          })}
        </ul>
        <div className="todo-field">
          <label htmlFor="exampleInputTask"></label>
          <input
              type="text"
              className="form-control"
              id="todo-input"
              placeholder="Enter task"
              value={ this.state.inputTask }
              onChange={ this.onChangeTask }
          >
          </input>
          <button
            type="button"
            className="btn btn-outline-success"
            onClick={ this.createTask }
          >Add</button>
        </div>
      </div>
    )
  }
}
