#!/bin/bash
set -e

REPO=$(git remote get-url origin | sed 's|.*github\.com[:/]\(.*\)\.git|\1|' | sed 's|.*github\.com[:/]\(.*\)|\1|')
OUTDIR="./firmware"
COMMIT=$(git rev-parse HEAD)

echo "Repo: $REPO"
echo "Commit: $COMMIT"

# Find the run for the current commit, waiting up to 30s for it to appear
echo "Looking for build run..."
RUN_ID=""
for i in $(seq 1 6); do
    RUN_ID=$(gh run list --repo "$REPO" --commit "$COMMIT" --limit 1 --json databaseId -q '.[0].databaseId' 2>/dev/null || true)
    [[ -n "$RUN_ID" ]] && break
    echo "  waiting for run to appear... ($((i * 5))s)"
    sleep 5
done

if [[ -z "$RUN_ID" ]]; then
    echo "No build run found for commit $COMMIT."
    echo "Make sure you've pushed this commit and GitHub Actions has started."
    exit 1
fi

echo "Watching run $RUN_ID..."
gh run watch "$RUN_ID" --repo "$REPO" --exit-status

echo "Downloading firmware..."
rm -rf "$OUTDIR"
gh run download "$RUN_ID" --repo "$REPO" --dir "$OUTDIR"

echo ""
echo "Done! Firmware files:"
find "$OUTDIR" -name "*.uf2" | sort
