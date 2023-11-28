#!/bin/bash

ENV=$(find . -type f -name .env)
. $ENV

TIME="10"
URL="https://api.telegram.org/bot$TELEGRAM_BOT_TOKEN/sendMessage"
TEXT="Status:+$CI_JOB_STATUS%0AJob:+$CI_JOB_NAME%0AStage:+$CI_JOB_STAGE%0APipeline:+$CI_PIPELINE_IID%0A%0AProject:+$CI_PROJECT_NAME%0AURL:+$CI_PROJECT_URL/pipelines/$CI_PIPELINE_ID/%0ABranch:+$CI_COMMIT_REF_SLUG%0AMessage:+$CI_COMMIT_MESSAGE"

curl -s --max-time $TIME -d "chat_id=$TELEGRAM_USER_ID&disable_web_page_preview=1&text=$TEXT" $URL > /dev/null
