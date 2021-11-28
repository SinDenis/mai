import React, { Component } from 'react';
import {connect} from 'react-redux';
import './book-list.css';
import BookListItem from "../book-list-item";
import { WithBookStoreService } from '../hoc';
import {bookAddedToCart, fetchBooks} from "../../actions";
import compose from "../../utils";
import Spinner from "../spinner/spinner";
import ErrorIndicator from "../error-indicator";

const BookList = ({ books, onAddedToCart }) => {

  return (
    <ul className="book-list">
      {
        books.map((book) => {

          return (
            <li key={ book.id }>
              <BookListItem
                onAddedToCart={ () => onAddedToCart(book.id) }
                book={ book }/>
            </li>
          );

        })
      }
    </ul>
  );

};

class BookListContainer extends Component {

  componentDidMount() {
    this.props.fetchBooks();
  }

  render() {

    const { books, loading, error, onAddedToCart } = this.props;

    if (error) {
      return <ErrorIndicator />
    }

    if (loading) {
      return <Spinner />
    }

    return <BookList books={ books } onAddedToCart={onAddedToCart} />
  }

}

const mapStateToProps = ({ books, loading, error }) => {

  return { books, loading, error };

};

const mapDispatchToProps = (dispatch, ownProps) => {

  const { bookstoreService } = ownProps;

  return {
    fetchBooks: fetchBooks(dispatch, bookstoreService),
    onAddedToCart: (id) => dispatch(bookAddedToCart(id))
  };
};

export default compose(
  WithBookStoreService(),
  connect(mapStateToProps, mapDispatchToProps)
)(BookListContainer);
