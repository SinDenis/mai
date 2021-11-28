import React from 'react';
import { Link } from 'react-router-dom';
import './header.css';

export default function header() {
  return (
    <nav className="navbar navbar-expand-lg navbar-dark bg-primary">
      <a className="navbar-brand" href="#">Todo</a>
      <button className="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarColor01" aria-controls="navbarColor01" aria-expanded="false" aria-label="Toggle navigation">
        <span className="navbar-toggler-icon"></span>
      </button>

      <div className="collapse navbar-collapse" id="navbarColor01">
        <ul className="navbar-nav mr-auto">
          {/* <li className="nav-item" id="home">
            <Link className="nav-link" to="/dashboard">Dashboard</Link>
          </li> */}
          <li className="nav-item" id="Login">
            <Link className="nav-link" to="/login">Login</Link>
          </li>
          <li className="nav-item active" id="Register">
            <Link className="nav-link" to="/register">Register</Link>
          </li>
          <li className="nav-item" id="Raiting">
            <Link className="nav-link" to="/raiting">Raiting</Link>
          </li>
        </ul>
        <form className="form-inline my-2 my-lg-0">
          <input className="form-control mr-sm-2" type="text" placeholder="Search"></input>
          <button className="btn btn-secondary my-2 my-sm-0" type="submit">Search</button>
        </form>
      </div>
    </nav>
  );
}
