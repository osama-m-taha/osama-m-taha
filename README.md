# OSAMA M. TAHA

**Computer Science and Engineering**  
[Egypt-Japan University of Science and Technology (E-JUST)](https://www.ejust.edu.eg/)

---

## About

I am studying **Computer Science and Engineering** at **E-JUST**, with a strong interest in **software engineering practice**: building full-stack systems end to end, keeping requirements traceable, and shipping work that is testable and maintainable—not just “demo-ready.”

On team projects I gravitate toward **architecture, auth, and platform work** (identity, RBAC, CI, Docker) while also caring about **product polish**—routing, admin flows, bilingual UI, and real-world edge cases (Unicode passwords, bulk imports, repeatable scheduling).

I use AI-assisted development thoughtfully: for exploration, implementation drafts, and tests, with **human review** on security, acceptance criteria, and what actually gets merged.

---

## Education & affiliation

| | |
|---|---|
| **Institution** | Egypt-Japan University of Science and Technology (E-JUST) |
| **Department** | Computer Science and Engineering |
| **Focus** | Software engineering, full-stack web systems |

---

## What I work on

- **Vertical slicing** — owning features across DB → API → UI → tests
- **Secure authentication** — JWT, RBAC, password policies, token invalidation
- **Internationalization** — English/Arabic (RTL), extensible locale setup, Arabic numerals in UI, German locale support
- **DevOps & quality** — GitHub Actions, Docker Compose, pytest pyramid, Playwright E2E
- **Accessible, practical UX** — admin guards, bulk actions, spreadsheet import (XLSX/XLS and common formats)

---

## Featured project

### Staff Scheduling & Shift Management System (SSSMS)

Full-stack staff scheduling platform built for **CSE323 — Software Engineering** (May 2026).

**Highlights**
- FastAPI + PostgreSQL backend with Alembic migrations, layered architecture (repositories/services), and Pydantic API contracts
- React + TypeScript frontend (Vite), role-based routing, light/dark themes
- Shifts, assignments, availability, attendance, shift swaps, roster/payroll reports, immutable audit log
- Recurring shifts with series grouping, bulk staff assignment, and manager bulk upload
- **react-i18next** localization (EN / AR / DE), RTL support, locale-aware dates
- Testing: unit, integration, and Playwright login E2E; CI with linting and broader non-E2E test runs

**Practices**
- Vertical slicing across a 3-person team
- Information hiding (ORM models never exposed at API boundary)
- 70/20/10 testing pyramid and requirement traceability (FR/HR/AU IDs)

*Repository:* Staff Scheduling & Shift Management System (private/course repo — add your link here)

**Typical ownership on this project:** Auth, users, JWT/RBAC, account & user-admin UI, Docker/CI/health infra, and cross-cutting improvements (i18n, routing, CI honesty, integration polish).

---

## Skills & tools

**Languages:** Python, TypeScript, SQL  

**Backend:** FastAPI, Pydantic, SQLAlchemy, Alembic, bcrypt/JWT, PostgreSQL  

**Frontend:** React, Vite, React Router, react-i18next, Tailwind CSS, Axios  

**Testing & automation:** pytest, Playwright, GitHub Actions, coverage/ruff  

**Other:** Docker Compose, Git, OpenAPI, spec-driven development (Gherkin/traceability)

---

## Interests

- Software engineering process (design → implementation → validation)
- API design and layered backend architecture
- Security basics done right (auth, RBAC, password handling)
- Multilingual and RTL-first web applications
- Developer experience: CI that matches the repo, clear docs, small reviewable changes

---

## How I like to work

- Start from **requirements IDs** and measurable acceptance criteria, not vague “fix auth” tasks
- Prefer **integration polish** and honest CI over large rewrites
- Iterate with **tests and builds** before calling a slice done
- Document decisions (traceability matrices, validation reports, AI usage logs where required)

---

## Contact

- **Email:** *your E-JUST or personal email*
- **GitHub:** *@your-username*
- **LinkedIn:** *profile URL*
- **Location:** Egypt (UTC+3)

---

<sub>Osama M. Taha · Computer Science and Engineering · E-JUST</sub>
