FROM openjdk:15-slim

ARG JAVA_OPTS
ARG JAR_FILE=build/libs/app.jar

COPY ${JAR_FILE} /app/app.jar

RUN groupadd bank-app && \
    useradd -g bank-app bank-app && \
    chown bank-app:bank-app /app/app.jar

CMD cd app && java ${JAVA_OPTA} -jar app.jar