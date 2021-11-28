import React from 'react';
import { BookStoreServiceConsumer } from "../bookstore-service-context";

const WithBookStoreService = () => (Wrapped) => {

  return (props) => {

    return (
      <BookStoreServiceConsumer>
        {
          (bookstoreService) => {
            return (<Wrapped { ...props } bookstoreService={ bookstoreService } />)
          }
        }
      </BookStoreServiceConsumer>
    );

  };

};

export default WithBookStoreService;
