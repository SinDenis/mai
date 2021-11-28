import React, {Component} from 'react';
import BookList from '../book-list';
import ShoppingCartTable from "../shopping-cart-table";

export default class HomePage extends Component {

  render() {
    return (
      <div>
        <BookList />
        <ShoppingCartTable />
      </div>
    );
  }

}
