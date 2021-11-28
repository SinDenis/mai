
const initialState = {
  books: [],
  loading: true,
  error: null,
  cartItems: [],
  orderTotal: 0
};

const reducer = (state = initialState, action) => {

  switch (action.type) {

    case 'FETCH_BOOKS_REQUEST':
      return {
        ...state,
        books: action.payload,
        loading: false,
        error: null
      };

    case 'FETCH_BOOKS_SUCCESS':
      return {
        ...state,
        books: [],
        loading: true,
        error: null
      };

    case 'FETCH_BOOKS_FAILURE':
      return {
        ...state,
        books: [],
        loading: false,
        error: action.payload
      };

    case 'BOOK_ADDED_TO_CART':

      const bookId = action.payload;
      const book = state.books.find(({ id }) => id === bookId);
      const itemIndex = state.cartItems.findIndex(({ id }) => id === bookId);
      const item = state.cartItems[itemIndex];

      let newItem;

      if (item) {
        newItem = {
          ...item,
          count: item.count + 1,
          total: item.total + book.price
        }
      } else {
        newItem = {
          id: book.id,
          name: book.title,
          count: 1,
          total: book.price
        }
      }

      if (itemIndex < 0) {
        return {
          ...state,
          cartItems: [
            ...state.cartItems,
            newItem
          ],
          orderTotal: state.orderTotal + book.price
        }
      }

      return {
        ...state,
        cartItems: [
          ...state.cartItems.slice(0, itemIndex),
          newItem,
          ...state.cartItems.slice(itemIndex + 1)
        ],
        orderTotal: state.orderTotal + book.price
      };

    default:
      return state;

  }

};

export default reducer;
