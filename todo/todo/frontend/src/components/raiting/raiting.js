import React, { Component } from 'react'

import './raiting.css';

export default class Raiting extends Component {

  state = {
    people: []
  }

  getPeople = () => {

    fetch('http://localhost:8000/api/user/', {
      method: 'GET',
      headers: {
        "Content-type": "application/json"
      }
    })

    .then((res) => {
      return res.json();
    })

    .then((body) => {
      this.setState(() => {
        return {
          people: [...body]
        }
      });
    })

    .catch((err) => {
      console.log(err.message);
    });

  }

  componentWillMount() {
    this.getPeople();
  }

  render() {

    return (
      
      <div className="raiting">
        <h3 className="raiting-title">User List</h3>
        <ul className="list-group raiting-list">
          {  this.state.people.map(({ username, level }, index) => {
            return <li key={ index } className="list-group-item d-flex justify-content-between align-items-center"><span>{ username }</span><span>{ level }</span></li>
          }) }
        </ul>
      </div>

    )

  }

}
