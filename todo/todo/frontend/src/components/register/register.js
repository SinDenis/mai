import React, { Component } from 'react';
import { Redirect, BrowserRouter, Switch } from 'react-router-dom';
import './register.css';

export default class Register extends Component {

  state = {
    username: '',
    email: '',
    password: '',
    isRegister: false
  }

  onSubmit = (event) => {
    event.preventDefault();
    fetch('http://localhost:8000/api/register/', {
      method: 'POST',
      headers: {
        "Content-type": "application/json"
      },
      body: JSON.stringify({
        username: this.state.username,
        email: this.state.email,
        password: this.state.password
      })
    })
    .then((res) => {
      return res.json();
    })
    .then((json) => {
      console.log(json);
    })
    .then(() => {
      this.setState(() => {
        return {
          username: '',
          email: '',
          password: '',
          isRegister: true
        }
      });
    })
    .catch((err) => {
      console.log(err.message);
    });
  }

  onChangeUsername = (event) => {
    this.setState({username: event.target.value});
  }

  onChangeEmail = () => {
    this.setState({email: event.target.value});
  }

  onChangePassword = () => {
    this.setState({password: event.target.value});
  }

  render() {
    if (this.state.isRegister) {
      return <Redirect to="/login" />
    }
    return (
      <form className="register" onSubmit={this.onSubmit}>
        <legend>Registration</legend>
        <div className="form-group">
          <label htmlFor="exampleInputUsername">Username</label>
          <input
            type="text"
            className="form-control"
            id="exampleInputUsername"
            placeholder="Enter username"
            value={this.state.username}
            onChange={this.onChangeUsername}
          >
          </input>
        </div>
        <div className="form-group">
          <label htmlFor="exampleInputEmail1">Email address</label>
          <input
            type="email"
            className="form-control"
            id="exampleInputEmail1"
            aria-describedby="emailHelp"
            placeholder="Enter email"
            value={this.state.email}
            onChange={this.onChangeEmail}
          >
          </input>
        </div>
        <div className="form-group">
          <label htmlFor="exampleInputPassword1">Password</label>
          <input
            type="password"
            className="form-control"
            id="exampleInputPassword1"
            placeholder="Password"
            value={this.state.password}
            onChange={this.onChangePassword}
          >
          </input>
        </div>
        <button type="submit" className="btn btn-primary">Submit</button>
      </form>
    );
  }
}
